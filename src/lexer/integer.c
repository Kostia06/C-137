#include "private.h"

static int size = 0;

void lexer_new_integer(Lexer* lexer){
    lexer->token->type = T_INTEGER;
    lexer->token->value.integer = lexer->current_char - '0';
    lexer->token->column = lexer->column;
    lexer->token->size = 1;
    lexer_advance(lexer);
}
void lexer_add_digit(Lexer* lexer){
    lexer->token->value.integer = lexer->token->value.integer * 10 + lexer->current_char - '0';
    lexer->token->size++;
    lexer_advance(lexer);
}
void lexer_add_dot(Lexer* lexer){
    lexer->token->type = T_FLOAT;
    lexer->token->size++;
    size = 1;
    lexer_advance(lexer);
}
void lexer_add_decimal(Lexer* lexer){
    lexer->token->value.integer += (lexer->current_char - '0') / pow(10,size++);
    lexer->token->size++;
    lexer_advance(lexer);
}
void lexer_error_decimal(Lexer* lexer){
    char* error[] = {"Invalid decimal",NULL};
    ERROR(1,lexer->line,lexer->column,1,error,lexer->scope); 
}
