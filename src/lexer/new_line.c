#include "private.h"

void lexer_new_line(Lexer* lexer){
    lexer->token->type = T_NEW_LINE;
    lexer->token->value.integer = 0;
    lexer->line++;
    lexer->column = 0;
    lexer_advance(lexer);
}
void lexer_add_new_line(Lexer* lexer){
    lexer->token->value.string = realloc(lexer->token->value.string,sizeof(char)*(strlen(lexer->token->value.string)+2));
    lexer->token->value.string[strlen(lexer->token->value.string)] = '\n';
    lexer->column = 0;
    lexer->line++;
    lexer_advance(lexer);
}
void lexer_count_space(Lexer* lexer){
    lexer->token->value.integer++;
    lexer_advance(lexer);
}
