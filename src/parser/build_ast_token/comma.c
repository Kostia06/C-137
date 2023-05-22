#include "../private.h"



void parser_token_comma(Parser* parser){
    if(parser->ast->type == P_EXPRESSION){return;}
    parser->ast->type = P_COMMA;
    parser->ast->actual_type = parser->ast->type;
    parser->ast->value.string = parser->token->value.string;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
}
