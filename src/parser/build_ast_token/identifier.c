#include "../private.h"

void parser_token_identifier(Parser* parser){
    parser->ast->type = P_IDENTIFIER;
    parser->ast->actual_type = P_IDENTIFIER;
    parser->ast->value.string = parser->token->value.string;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = parser->token->size;
}
void parser_empty_identifier(Parser* parser){
    AST* last = vector_get(parser->main_ast->children, parser->main_ast->children->size - 1);
    if(last != NULL && last->type == P_TYPE){
        last = vector_pop(parser->main_ast->children);
        AST* ast = malloc(sizeof(AST));
        ast->type = P_TYPE_AND_IDENTIFIER;
        ast->actual_type = P_TYPE_AND_IDENTIFIER;
        ast->line = last->line;
        ast->column = last->column;
        ast->size = last->size;
        ast->children = vector_init();
        vector_add(ast->children,last);
        vector_add(ast->children,parser->ast);
        vector_add(parser->main_ast->children,ast);
        parser->ast = malloc(sizeof(AST));
        parser_advance(parser);
        return;
    }
    parser_add_ast_to_main(parser);
}
