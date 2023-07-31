#include "../private.h"

// create new swap node by collecting everyhting until the end
void parser_macro_swap_init(Parser* parser){
    Node* current = parser->current_node;
    if(current->children == NULL){
        current->children = vector_init(parser->compiler->memory);
    }
    int count = 1;
    Node* node;
    while(count && parser->index < (int)parser->nodes->size){
        node = vector_pop_by_index(parser->nodes,parser->index);
        if(node->type > MACROS_START && node->type < MACROS_END && node->type != MACRO_END){count++;}
        else if(node->type == MACRO_END){count--;}
        if(count == 0){FREE_NODE(parser->compiler->memory,node);break;}
        vector_add(current->children,node);
    }
    current->size = node->index - current->index + node->size; 
    vector_add(parser->compiler->swaps,current);
    mem_free(parser->compiler->memory,parser->cmd->children);
    mem_free(parser->compiler->memory,parser->cmd);
    parser_reset_cmd(parser);
}
