#include "../private.h"

// Just handles simple symbols or tokens without values
void parser_token_symbol(Parser* parser){
    parser->ast->type = parser->token->type;
    parser->ast->actual_type = parser->token->type;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = parser->token->size;
    // clean up
    parser_free_token(parser);
}
// Equal is a special case
void parser_token_equal(Parser* parser){
    parser->ast->type = EQUAL;
    parser->ast->actual_type = EQUAL;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
}
