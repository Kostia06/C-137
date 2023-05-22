#include "../private.h"

void parser_switch_ast(AST* ast1, AST* ast2){
    AST temp;
    memcpy(&temp,ast1,sizeof(AST));
    memcpy(ast1,ast2,sizeof(AST));
    memcpy(ast2,&temp,sizeof(AST));
}
void parser_advance(Parser* parser){parser->token = vector_get(parser->tokens,parser->index++);}
void parser_add_main_to_asts(Parser* parser){
    AST* ast = parser->main_ast;
    if(parser->main_ast->type == P_EMPTY && parser->main_ast->children->size == 1){
        ast = vector_pop(parser->main_ast->children);
        free(parser->main_ast);
    }
    vector_add(parser->asts,ast);
    parser->ast = malloc(sizeof(AST));
    parser->main_ast = malloc(sizeof(AST));
    parser->main_ast->children = vector_init();
    parser_advance(parser);
}
void parser_add_ast_to_asts(Parser* parser){
    vector_add(parser->asts,parser->ast);
    parser->ast = malloc(sizeof(AST));
    parser_advance(parser);
}
void parser_add_ast_to_main(Parser* parser){
    vector_add(parser->main_ast->children,parser->ast);
    parser->ast = malloc(sizeof(AST));
    parser_advance(parser);
}
void parser_reset_main(Parser* parser){
    parser->main_ast = malloc(sizeof(AST));
    parser->main_ast->type = parser->init_type;
    parser->main_ast->actual_type = parser->init_type;
    parser->main_ast->children = vector_init();
}
