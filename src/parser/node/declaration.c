#include "../private.h"

// the end of the assignment
void parser_declaration_end(Parser* parser){
    parser->cmd->size = parser->current_node->index - parser->cmd->index;
    parser_add_cmd_to_cmds(parser);
    // clean up
    FREE_NODE(parser->compiler->memory,parser->current_node);
}
// get the value
void parser_declaration_value(Parser* parser){
    parser->hold_cmd = parser->cmd;
    parser->cmd = mem_init(parser->compiler->memory,sizeof(Node));
    parser->cmd->children = vector_init();
    parser->cmd->index = parser->current_node->index;
    parser->cmd->type = EXPRESSION;
    // clean up
    FREE_NODE(parser->compiler->memory,parser->current_node);
}
