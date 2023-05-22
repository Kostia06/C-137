#include "../private.h"

void parser_token_keyword_with_expression(Parser* parser){
    parser->main_ast->type = parser->token->type + parser->keyword_offset;
    parser->main_ast->actual_type = parser->main_ast->type;
    parser->main_ast->line = parser->token->line;
    parser->main_ast->column = parser->token->column;
    parser->main_ast->size = parser->token->size;
    parser->main_ast->children = vector_init();
    parser->hold_ast = parser->main_ast;

    parser->main_ast = malloc(sizeof(AST));
    parser->main_ast->type = P_EXPRESSION;
    parser->main_ast->actual_type = P_EXPRESSION;
    parser->main_ast->line = parser->token->line;
    parser->main_ast->column = parser->token->column + parser->token->size;
    parser->main_ast->size = 1;
    parser->main_ast->children = vector_init();

    parser_advance(parser);
}
void parser_token_keyword(Parser* parser){
    parser->main_ast->type = parser->token->type + parser->keyword_offset;
    parser->main_ast->actual_type = parser->main_ast->type;
    parser->main_ast->line = parser->token->line;
    parser->main_ast->column = parser->token->column;
    parser->main_ast->size = parser->token->size;
    parser->main_ast->children = vector_init();
    parser_advance(parser);
}

