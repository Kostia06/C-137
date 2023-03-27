#include "include.h"

static TokenBlock* new_block(int type,char* scope){
    TokenBlock* block = malloc(sizeof(TokenBlock));
    block->type = type;
    block->block_size = 0;
    block->blocks = malloc(sizeof(TokenBlock*));
    block->token_size = 0;
    block->tokens = malloc(sizeof(Token*));
    block->scope = scope;
    return block;
}
static TokenBlock** level_block(TokenBlock** blocks, size_t size,int index,size_t* return_size){
    TokenBlock** new_blocks = malloc(sizeof(TokenBlock*));
    size_t new_size = 0;
    int i = 0;
    while(i<(int)size){
        TokenBlock* block = blocks[i++];
        block->index = index;
        switch(block->type){
            case FUNCTION:
            case IF:case ELIF:
            case FOR:case WHILE:{
                TokenBlock** hold = malloc(sizeof(TokenBlock*));
                size_t hold_size = 0;
                int spacing = block->spacing;
                ERROR(i >= (int)size,block->line,(char*[]){"Block did not receive any blocks",NULL},__func__,block->scope);
                if(blocks[i]->type == FUNCTION_START){
                    int count = 1;
                    i++;
                    for(;i<(int)size;i++){
                        if(blocks[i]->type == FUNCTION_START){count++;}
                        else if(blocks[i]->type == FUNCTION_END){count--;}
                        if(!count){break;}
                        hold = realloc(hold,sizeof(TokenBlock*) * (hold_size + 1));
                        hold[hold_size++] = blocks[i];
                    }
                    i++;
                    ERROR(count>0,block->line,(char*[]){"Unmatched \"",TYPE(FUNCTION_END),"\"",NULL},__func__,block->scope);
                }
                else{
                    for(;i<(int)size;i++){
                        if(blocks[i]->spacing <= spacing){break;}
                        hold = realloc(hold,sizeof(TokenBlock*) * (hold_size + 1));
                        hold[hold_size++] = blocks[i];
                    }
                }
                ERROR(hold_size == 0,block->line,(char*[]){"Block did not receive any tokens",NULL},__func__,block->scope);
                hold = level_block(hold,hold_size,index+1,&hold_size);
                for(int i=0;i<(int)hold_size;i++){
                    block->blocks = realloc(block->blocks,sizeof(TokenBlock*) * (block->block_size + 1));
                    block->blocks[block->block_size++] = hold[i];
                    if(block->type == FUNCTION){hold[i]->scope = SINGLE_STRING((char*[]){block->scope,"!~!_fn",NULL});}
                }
            }
            default:{
                ERROR(block->type == FUNCTION_START || block->type == FUNCTION_END,block->line,(char*[]){"Block type not recognized",NULL},__func__,block->scope);
                new_blocks = realloc(new_blocks,sizeof(TokenBlock*) * (new_size + 1));
                new_blocks[new_size++] = block;
                break;
            }
        }
    }
    *return_size = new_size;
    return new_blocks;
}

TokenBlock** create_blocks(Token** tokens,size_t size,char* scope,size_t* return_size){
    TokenBlock** blocks = malloc(sizeof(TokenBlock*));
    size_t block_size = 0;
    Token** hold = malloc(sizeof(Token*));
    size_t hold_size = 0;
    size_t index = 0,type = EMPTY;
    int next_spacing = 0;
    while(index < size){
        Token* current_token = tokens[index++];
        if(current_token->type == NEW_LINE || current_token->type == SEMICOLON || (index < size && tokens[index]->type == FUNCTION_START)){
            if(hold_size == 0){continue;}
            ERROR(type == EMPTY,current_token->line,(char*[]){"Line did not receive a parser type",NULL},__func__,scope);
            TokenBlock* block = new_block(type,scope);
            block->scope = scope;
            block->spacing = next_spacing;
            block->token_size = hold_size;
            block->tokens = realloc(block->tokens,sizeof(Token*) * hold_size);
            block->tokens = hold;
            block->line = current_token->type == NEW_LINE ? current_token->line-1 : current_token->line;

            blocks = realloc(blocks,sizeof(TokenBlock*) * (block_size + 1));
            blocks[block_size++] = block;

            hold=malloc(sizeof(Token*));
            hold_size = 0;
            type = EMPTY;
            next_spacing = (index < size && tokens[index]->type == FUNCTION_START) ? *(int*)tokens[index]->value : *(int*)current_token->value;
            continue;
        }
        if(current_token->type == FUNCTION_START || current_token->type == FUNCTION_END){
            TokenBlock* block = new_block(current_token->type,scope);
            block->type = current_token->type;
            block->line = current_token->line;
            blocks = realloc(blocks,sizeof(TokenBlock*) * (block_size + 1));
            blocks[block_size++] = block;
            continue;
        }
        if(current_token->type >= KEYWORD_START && current_token->type <= KEYWORD_END){
            ERROR(type != EMPTY,current_token->line,(char*[]){"Too many parser types",NULL},__func__,scope);
            type = current_token->type;
            continue;
        }
        hold = realloc(hold,sizeof(Token*) * (hold_size + 1));
        hold[hold_size++] = current_token;
    }
    blocks = level_block(blocks,block_size,0,&block_size);
    *return_size = block_size;
    return blocks;
}
