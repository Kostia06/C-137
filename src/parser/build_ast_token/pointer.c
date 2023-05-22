#include "../private.h"

void parser_token_pointer(Parser* parser){
    parser->ast->type = P_POINTER;
    parser->ast->actual_type = P_POINTER;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = parser->token->size;
}
