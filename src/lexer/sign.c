#include "private.h"

static int single_sign_types[T_END] ={
    [T_ARGUMENT_END] = 1,
    [T_ARGUMENT_START] = 1,
    [T_ARRAY_END] = 1,
    [T_ARRAY_START] = 1,
    [T_SEMICOLON] = 1,
    [T_COMMA] = 1,
    [T_ADD] = 1,
    [T_MUL] = 1, 
    [T_DIV] = 1,
};
void lexer_new_sign(Lexer* lexer){
    lexer->token->type = T_SIGN;
    lexer->token->column = lexer->column;
    lexer->token->value.string = malloc(sizeof(char)*2);
}
void lexer_add_char_to_sign(Lexer* lexer){
    lexer_add_char(lexer);
    for(int i = 0;i < SIGN_COUNT;i++){
        if(!strcmp(lexer->token->value.string,signs[i].name)){
            int function = single_sign_types[signs[i].type];
            if(function){
                lexer->token->type = signs[i].type;
                lexer_reset_token(lexer);
                return;
            }
            break;
        }
    }
}
void lexer_skip(Lexer* lexer){
    lexer_advance(lexer);lexer_advance(lexer);
}
void lexer_semicolon(Lexer* lexer){
    lexer->token->type = T_SEMICOLON;
    lexer_advance(lexer);lexer_reset_token(lexer);
}
void lexer_check_sign(Lexer* lexer){
    for(int i = 0;i < SIGN_COUNT;i++){
        if(!strcmp(lexer->token->value.string,signs[i].name)){
            lexer->token->type = signs[i].type;
            break;
        }
    }
    {
        char* error[] = {"Unknown sign",lexer->token->value.string,NULL};
        int e_condition = lexer->token->type == T_SIGN;
        ERROR(e_condition, lexer->token->line,lexer->token->column,lexer->token->size,error,lexer->scope);
    }
    lexer_reset_token(lexer);
}
