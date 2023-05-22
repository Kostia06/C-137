#include "../private.h"

#define DEBUG_PARAMETER     0

static void parser_check_argument(Parser* parser){
    int type = P_EMPTY;
    switch(parser->ast->type){
        case P_PARAMETERS:{type = P_TYPE_AND_IDENTIFIER;break;}
        case P_TUPLE_WITH_TYPES:{type = P_TYPE;break;}
        case P_TUPLE_WITH_VALUES:{type = P_EXPRESSION;break;}
        case P_TUPLE_WITH_IDENTIFIERS:{type = P_IDENTIFIER;break;}
    }
    for(int i=0;i<(int)parser->ast->children->size;i++){
        AST* ast = vector_get(parser->ast->children,i);
        char* error[] = {"Expected a ",PARSER_TYPE(type),", but got a ",PARSER_TYPE(ast->type),NULL};
        int e_condition = ast->type != type;
        ERROR(e_condition,ast->line,ast->column,ast->size,error,parser->scope);
    }
}
void parser_token_end_argument(Parser* parser){
    parser->ast->type = P_END_OF_ARGUMENT;
    parser->ast->actual_type = P_END_OF_ARGUMENT;
    parser->ast->line = parser->token->line; 
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
}
void parser_token_argument(Parser* parser){
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
    int type = P_EMPTY;
    AST* last = vector_get(parser->main_ast->children,parser->main_ast->size);
    if(parser->main_ast->type == P_EMPTY && parser->main_ast->children->size == 0){
        parser->ast->type = P_TUPLE_WITH_IDENTIFIERS;
    }
    else if(parser->main_ast->type == P_FN && last != NULL && last->type == P_POINTER){
        parser->ast->type = P_TUPLE_WITH_TYPES;
    }
    else if(parser->main_ast->type == P_FN){
        parser->ast->type = P_PARAMETERS;
    }
    else if(parser->main_ast->type == P_EXPRESSION){
        parser->ast->type = P_TUPLE_WITH_VALUES;
        type = P_EXPRESSION;
    }
    parser->ast->actual_type = parser->ast->type;
    int count = 1;
    Vector* tokens = vector_init();
    parser_advance(parser);
    while(parser->index <= parser->tokens->size){
        Token* token = parser->token;
        if(token->type == T_NEW_LINE){parser_advance(parser);continue;}
        vector_add(tokens,token);
        if(token->type == T_ARGUMENT_START){count++;}
        else if(token->type == T_ARGUMENT_END){count--;}
        if(count == 0){break;}
        parser_advance(parser);
    }
    parser->ast->children = parse(tokens,type,parser->scope);
    parser_check_argument(parser);
    if(last != NULL && last->type == P_IDENTIFIER){
        if(last->children == NULL){last->children = vector_init();}
        vector_add(last->children,parser->ast);
        parser->ast = malloc(sizeof(AST));
        parser_advance(parser);
    }
    else if(parser->main_ast->type == P_EXPRESSION && parser->ast->children->size == 1){
        parser->ast = vector_get(parser->ast->children,0);
    }
    else if(parser->main_ast->type == P_EXPRESSION && parser->main_ast->children->size == 0){
        parser_switch_ast(parser->main_ast,parser->hold_ast);
        parser->hold_ast = NULL;
    }
}
