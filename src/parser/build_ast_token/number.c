#include "../private.h"

void parser_token_number(Parser* parser){
    parser->ast->type = parser->token->type == T_INTEGER ? P_INTEGER : P_FLOAT;
    parser->ast->actual_type = parser->ast->type;
    parser->ast->value.integer = parser->token->value.integer;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = parser->token->size;
}
