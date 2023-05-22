#include "private.h"


void lexer_new_string(Lexer* lexer){
    char c = lexer->current_char;
    lexer->token->type = c == '`' ? T_B_STRING : c == '"' ? T_D_STRING : T_S_STRING;
    lexer->token->value.string = malloc(sizeof(char)*2);
    lexer->token->line = lexer->line;
    lexer->token->column = lexer->column;
    lexer->token->size = 0;
    lexer_advance(lexer);    
}
void lexer_single_string_add_char(Lexer* lexer){
    if(lexer->current_char == '\''){
        char c = lexer->token->value.string[0];
        lexer->token->value.integer = c;
        lexer->token->type = T_INTEGER;
        lexer_reset_token(lexer);
        lexer_advance(lexer);
        return;
    }
    if(lexer->current_char == '\\'){
        lexer_add_char(lexer);
        return;
    }
    char* error[] = {"Single string must contain only one charcter",NULL};
    int e_condition = lexer->token->size == 1;
    ERROR(e_condition,lexer->token->line,lexer->token->column,3,error,lexer->scope);
    lexer_add_char(lexer);
}
