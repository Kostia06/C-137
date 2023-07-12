#include "../private.h"

// handle the identifier in the empty state
void parser_empty_identifier(Parser* parser){
    parser->cmd->type = DECLARATION;
    parser->cmd->index = parser->current_node->index;
    parser->cmd->children = vector_init();
    parser_add_node_to_cmd(parser);
}
