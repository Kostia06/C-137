#include "../private.h"

// Create integer/float AST
void parser_token_integer(Parser* parser){
    parser->ast->type = parser->token->type;
    parser->ast->actual_type = parser->token->type;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = parser->token->size;
    parser->ast->value.string = mem_copy(parser->memory,parser->token->value.string);
    // clean up
    parser_free_token(parser);
}
