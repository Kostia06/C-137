
#include "../private.h"


void parser_token_array(Parser* parser){
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
    parser->ast->type = P_ARRAY;
    parser->ast->actual_type = P_ARRAY;
    parser->ast->children = vector_init();
    int count = 1;
    Vector* tokens = vector_init();
    parser_advance(parser);
    {
        char* error[] = {"Expected fixed integer",NULL};
        int e_condition = parser->token->type != T_INTEGER;
        ERROR(e_condition,parser->token->line,parser->token->column,parser->token->size,error,parser->scope);
    }
    AST* ast = malloc(sizeof(AST));
    ast->type = P_INTEGER;
    ast->actual_type = P_INTEGER;
    ast->line = parser->token->line;
    ast->column = parser->token->column;
    ast->size = parser->token->size;
    ast->value.integer = parser->token->value.integer;
    parser_advance(parser);
    {
        char* error[] = {"Expected the end of the fixed array",NULL};
        int e_condition = parser->token->type != T_ARRAY_END;
        ERROR(e_condition,parser->token->line,parser->token->column,parser->token->size,error,parser->scope);
    }
    vector_add(parser->ast->children,ast);

}

