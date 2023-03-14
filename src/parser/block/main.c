#include "include.h"

typedef struct{
    Node** nodes;
    size_t node_size;
}Replace;

static NodeBlock* new_block(int type,char* scope){
    NodeBlock* block = malloc(sizeof(NodeBlock));
    block->type = type;
    block->block_size = 0;
    block->blocks = malloc(sizeof(NodeBlock*));
    block->node_size = 0;
    block->nodes = malloc(sizeof(Node*));
    block->scope = scope;
    return block;
}
static Node** add_token_to_array(Node** array,Node* value,int size,int index){
    Node** new_array = malloc(sizeof(Node*)*(size+1));
    for(int i = 0;i < size+1;i++){
        if(i < index){new_array[i] = array[i];}
        else if(i == index){new_array[i] = value;}
        else{new_array[i] = array[i-1];}
    }
    return new_array;
}
static NodeBlock** level_block(NodeBlock** blocks, size_t size,int index,size_t* return_size){
    NodeBlock** new_blocks = malloc(sizeof(NodeBlock*));
    size_t new_size = 0;
    int i = 0;
    while(i<(int)size){
        NodeBlock* block = blocks[i++];
        block->index = index;
        switch(block->type){
            case FN:
            case IF:case ELIF:
            case FOR:case WHILE:{
                NodeBlock** hold = malloc(sizeof(NodeBlock*));
                size_t hold_size = 0;
                int spacing = block->spacing;
                ERROR(i >= (int)size,block->line,(char*[]){"Block did not receive any nodes",NULL},__func__,block->scope);
                if(blocks[i]->type == LBRACE){
                    int count = 1;
                    i++;
                    for(;i<(int)size;i++){
                        if(blocks[i]->type == LBRACE){count++;}
                        else if(blocks[i]->type == RBRACE){count--;}
                        if(!count){break;}
                        hold = realloc(hold,sizeof(NodeBlock*) * (hold_size + 1));
                        hold[hold_size++] = blocks[i];
                    }
                    i++;
                    ERROR(count>0,block->line,(char*[]){"Unmatched \"",TYPE(RBRACE),"\"",NULL},__func__,block->scope);
                }
                else{
                    for(;i<(int)size;i++){
                        if(blocks[i]->spacing <= spacing){break;}
                        hold = realloc(hold,sizeof(NodeBlock*) * (hold_size + 1));
                        hold[hold_size++] = blocks[i];
                    }
                }
                ERROR(hold_size == 0,block->line,(char*[]){"Block did not receive any nodes",NULL},__func__,block->scope);
                hold = level_block(hold,hold_size,index+1,&hold_size);
                for(int i=0;i<(int)hold_size;i++){
                    block->blocks = realloc(block->blocks,sizeof(NodeBlock*) * (block->block_size + 1));
                    block->blocks[block->block_size++] = hold[i];
                    if(block->type == FN){hold[i]->scope = SINGLE_STRING((char*[]){block->scope,"!~!_fn",NULL});}
                }
            }
            default:{
                ERROR(block->type == LBRACE || block->type == RBRACE,block->line,(char*[]){"Block type not recognized",NULL},__func__,block->scope);
                new_blocks = realloc(new_blocks,sizeof(NodeBlock*) * (new_size + 1));
                new_blocks[new_size++] = block;
                break;
            }
        }
    }
    *return_size = new_size;
    return new_blocks;
}

NodeBlock** create_blocks(Node** nodes,size_t size,char* scope,size_t* return_size){
    Replace* replacements[MAX_HASH_SIZE] = {NULL};
    NodeBlock** blocks = malloc(sizeof(NodeBlock*));
    size_t block_size = 0;
    Node** hold = malloc(sizeof(Node*));
    size_t hold_size = 0;
    size_t index = 0,type = EMPTY;
    int next_spacing = 0;
    while(index < size){
        Node* current_node = nodes[index++];
        if(current_node->type == NEW_LINE || current_node->type == SEMICOLON || (index < size && nodes[index]->type == LBRACE)){
            if(hold_size == 0){continue;}
            ERROR(type == EMPTY,current_node->line,(char*[]){"Line did not receive a parser type",NULL},__func__,scope);
            NodeBlock* block = new_block(type,scope);
            block->scope = scope;
            block->spacing = next_spacing;
            block->node_size = hold_size;
            block->nodes = realloc(block->nodes,sizeof(Node*) * hold_size);
            block->nodes = hold;
            block->line = current_node->type == NEW_LINE ? current_node->line-1 : current_node->line;

            blocks = realloc(blocks,sizeof(NodeBlock*) * (block_size + 1));
            blocks[block_size++] = block;

            hold=malloc(sizeof(Node*));
            hold_size = 0;
            type = EMPTY;
            next_spacing = (index < size && nodes[index]->type == LBRACE) ? *(int*)nodes[index]->value : *(int*)current_node->value;
            continue;
        }
        if(current_node->type == LBRACE || current_node->type == RBRACE){
            NodeBlock* block = new_block(current_node->type,scope);
            block->type = current_node->type;
            block->line = current_node->line;
            blocks = realloc(blocks,sizeof(NodeBlock*) * (block_size + 1));
            blocks[block_size++] = block;
            continue;
        }   
        if(current_node->type >= KEYWORD_START && current_node->type <= KEYWORD_END){
            ERROR(type != EMPTY,current_node->line,(char*[]){"Too many parser types",NULL},__func__,scope);
            type = current_node->type;
            continue;
        }
        if(current_node->type == MACRO_REPLACE){
            ERROR(type != EMPTY,current_node->line,(char*[]){"Too many parser types",NULL},__func__,scope);
            Node** replace = malloc(sizeof(Node*));
            size_t replace_size = 0;
            ERROR(index >= size || nodes[index]->type != IDENTIFIER,current_node->line,(char*[]){"Expected a custom token after const",NULL},__func__,scope);
            char* name = nodes[index]->value;
            index++;
            int current_line = current_node->line;
            while(index < size){
                if(nodes[index]->type == NEW_LINE){index++;break;}
                replace = realloc(replace,sizeof(Node*) * (replace_size + 1));
                replace[replace_size++] = nodes[index++];
            }
            ERROR(replace_size == 0,current_node->line,(char*[]){"Expected a value after macro replace",NULL},__func__,scope);
            Replace* new_replace = malloc(sizeof(Replace));
            new_replace->nodes = replace;
            new_replace->node_size = replace_size;
            int id = hash_id(name);
            ERROR(replacements[id] != NULL,current_node->line,(char*[]){"Constant already exists",NULL},__func__,scope);
            replacements[id] = new_replace;
            continue;
        }
        else if(current_node->type == IDENTIFIER){
            char* name = current_node->value;
            int id = hash_id(name);
            if(replacements[id] != NULL){
                Replace* replace_ = replacements[id];
                for(int i = 0;i < (int)replace_->node_size;i++){
                    replace_->nodes[i]->line = current_node->line;
                    nodes = add_token_to_array(nodes,replace_->nodes[i],size,index+i);
                    size++;
                }
                continue;
            }
        }

        hold = realloc(hold,sizeof(Node*) * (hold_size + 1));
        hold[hold_size++] = current_node;
    }
    blocks = level_block(blocks,block_size,0,&block_size);
    *return_size = block_size;
    return blocks;
}