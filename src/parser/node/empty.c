#include "../private.h"

// handle the identifier in the empty state
void parser_empty_identifier(Parser* parser){
    parser->cmd->type = DECLARATION;
    parser->cmd->index = parser->current_node->index;
    if(!parser->cmd->children){
        parser->cmd->children = vector_init(parser->compiler->memory);
    }
    parser_add_node_to_cmd(parser);
}
