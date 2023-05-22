#include "../private.h"

void parser_token_equal(Parser* parser){
    AST* last = vector_get(parser->main_ast->children, parser->main_ast->children->size - 1); 
    {
        char* error[] = {"Variable got no name",NULL};
        int e_condition = last == NULL || (last->type != P_IDENTIFIER && last->type != P_TYPE_AND_IDENTIFIER && last->type != P_TUPLE_WITH_IDENTIFIERS);
        ERROR(e_condition,parser->token->line,parser->token->column,parser->token->size,error,parser->scope);
    }
    parser->main_ast->type = P_VARIABLE;
    parser->main_ast->actual_type = P_VARIABLE;
    parser->hold_ast = parser->main_ast;
    parser_reset_main(parser);
    parser->main_ast->type = P_EXPRESSION;
    parser->main_ast->actual_type = P_EXPRESSION;
    parser_advance(parser);
}
