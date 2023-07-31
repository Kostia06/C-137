#include "../private.h"

static Node* create_copy(Parser* parser,Node* node, int index, int size){
    Node* copy = mem_copy(parser->compiler->memory,node,sizeof(Node));
    copy->index = index;
    copy->size = size;
    return copy;
}
// swap the current node with the swap
void parser_special_swap(Parser* parser){
    // gets the info from the current node
    char* name = parser->current_node->value.string;
    int index = parser->current_node->index;
    int size = parser->current_node->size;
    // search for the swap
    for(int i =0;i<parser->compiler->swaps->size;i++){
        Node* swap = vector_get(parser->compiler->swaps,i);
        if(!strcmp(swap->value.string,name)){
            // remove the current node to replace it with the swap
            FREE_NODE(parser->compiler->memory,parser->current_node);
            vector_remove(parser->nodes,--parser->index);
            // add the children of the swap
            for(int j=swap->children->size-1;j>=0;j--){
                Node* child = vector_get(swap->children,j);
                Node* copy = create_copy(parser,child,index,size); 
                vector_add_by_index(parser->nodes,copy,parser->index);
            }
            parser->current_node = NULL;
            for(int j=parser->index;j<parser->nodes->size;j++){
                PRINT_NODE(vector_get(parser->nodes,j),0);
            }
            return;
        }
    }
}
// give name to the command
void parser_keyword_name(Parser* parser){
    char* name = mem_copy(parser->compiler->memory,parser->current_node->value.string,sizeof(char*));
    parser->cmd->value.string = name;
    parser->cmd->size = parser->cmd->size + parser->current_node->size;
    // clean up
    FREE_NODE(parser->compiler->memory,parser->current_node);
}
// start of a new command without value
void parser_keyword(Parser* parser){
    parser->cmd->type = parser->current_node->type;
    parser->cmd->index = parser->current_node->index;
    parser->cmd->size = parser->current_node->size;
    if(parser->current_node->type == IDENTIFIER){
        char* name = mem_copy(parser->compiler->memory,parser->current_node->value.string,sizeof(char*));
        parser->cmd->value.string = name;
    }
    if(parser->cmd->children == NULL){
        parser->cmd->children = vector_init(parser->compiler->memory);
    }
    // clean up
    FREE_NODE(parser->compiler->memory,parser->current_node);
}
// start of a new command with value
void parser_keyword_with_value(Parser* parser){
    // set up the command
    parser->cmd->type = parser->current_node->type;
    parser->cmd->index = parser->current_node->index;
    if(parser->cmd->children == NULL){
        parser->cmd->children = vector_init(parser->compiler->memory);
    }
    // set up the value
    parser->hold_cmd = parser->cmd;
    parser->cmd = mem_init(parser->compiler->memory,sizeof(Node));
    parser->cmd->type = EXPRESSION;
    parser->cmd->index = parser->current_node->index;
    if(!parser->cmd->children){
        parser->cmd->children = vector_init(parser->compiler->memory);
    }
    // clean up
    FREE_NODE(parser->compiler->memory,parser->current_node);
}
