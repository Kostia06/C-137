#include "../private.h"

// When equal sign then change to assignment
void parser_variable_equal(Parser* parser){
    parser->hold_cmd = parser->cmd;
    parser->cmd = mem_init(parser->memory, sizeof(AST));
    parser->cmd->type = EXPRESSION;
    parser->cmd->actual_type = EXPRESSION;
    parser->cmd->children = vector_init();
    mem_free(parser->memory,parser->ast);
    parser->ast = mem_init(parser->memory, sizeof(AST));
    // clean up
    parser_free_token(parser);
}
