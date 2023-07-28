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
        Node* node = vector_get(parser->compiler->swaps,i);
        if(!strcmp(node->value.string,name)){
            // remove the current node to replace it with the swap
            FREE_NODE(parser->compiler->memory,parser->current_node);
            vector_remove(parser->nodes,--parser->index);
            Node* child = vector_get(node->children,0);
            Node* copy = create_copy(parser,child,index,size);
            parser->current_node = copy;
            // add the children of the swap
            for(int j=1;j<node->children->size;j++){
                child = vector_get(node->children,j);
                copy = create_copy(parser,child,index,size); 
                vector_add_by_index(parser->nodes,copy,parser->index);
            } 
            return;
        }
    }
}

// start of a new command without value
void parser_keyword(Parser* parser){
    parser->cmd->type = parser->current_node->type;
    parser->cmd->index = parser->current_node->index;
    if(!parser->cmd->children){
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
    if(!parser->cmd->children){
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
