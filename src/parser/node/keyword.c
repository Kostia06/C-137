#include "../private.h"

// start of a new command without value
void parser_keyword(Parser* parser){
    parser->cmd->type = parser->current_node->type;
    parser->cmd->index = parser->current_node->index;
    parser->cmd->children = vector_init();
    // clean up
    FREE_NODE(parser->memory,parser->current_node);
}
// start of a new command with value
void parser_keyword_with_value(Parser* parser){
    // set up the command
    parser->cmd->type = parser->current_node->type;
    parser->cmd->index = parser->current_node->index;
    parser->cmd->children = vector_init();
    // set up the value
    parser->hold_cmd = parser->cmd;
    parser->cmd = mem_init(parser->memory,sizeof(Node));
    parser->cmd->type = EXPRESSION;
    parser->cmd->index = parser->current_node->index;
    parser->cmd->children = vector_init();
    // clean up
    FREE_NODE(parser->memory,parser->current_node);
}
