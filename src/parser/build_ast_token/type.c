#include "../private.h"

void parser_token_type(Parser* parser){
    parser->ast->type = P_TYPE;
    parser->ast->actual_type = parser->token->type + parser->type_offset;
    parser->ast->value.string = parser->token->value.string;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = parser->token->size;   
    parser->ast->children = vector_init();
}
void parser_add_value_to_type(Parser* parser){
    AST* last = vector_get(parser->main_ast->children,parser->main_ast->children->size - 1);
    {
        char* error[] = {"Pointer got no type",NULL};
        int e_condition = last == NULL || last->type != P_TYPE;
        ERROR(e_condition,parser->ast->line,parser->ast->column,parser->ast->size,error,parser->scope);
    }
    vector_add(last->children,parser->ast);
    parser->ast = malloc(sizeof(AST));
    parser_advance(parser);
}
