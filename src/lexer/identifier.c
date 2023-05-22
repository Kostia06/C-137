#include "private.h"

static char lexer_escape_sequence(Lexer* lexer){
    char c = lexer->current_char;
    lexer_advance(lexer);
    switch(lexer->current_char){
        case 'n':c = '\n';break;
        case 't':c = '\t';break;
        case 'r':c = '\r';break;
        case 'b':c = '\b';break;
        case '\"':c = '\"';break;
        case '\'':c = '\'';break;
        case '\\':c = '\\';break;
        case '0':c = '\0';break;
    }
    return c;
}


void lexer_new_identifier(Lexer* lexer){
    lexer->token->type = T_IDENTIFIER;   
    lexer->token->column = lexer->column;
    lexer->token->size = 1;
    lexer->token->value.string = malloc(sizeof(char)*2);
    lexer->token->value.string[0] = lexer->current_char;
    lexer_advance(lexer);
}
void lexer_add_char(Lexer* lexer){
    char c = lexer->current_char;
    if(lexer->current_char == '\\'){c = lexer_escape_sequence(lexer);}
    else{lexer_advance(lexer);}
    lexer->token->value.string = realloc(lexer->token->value.string,sizeof(char)*(strlen(lexer->token->value.string)+2));
    lexer->token->value.string[strlen(lexer->token->value.string)] = c;
    if(lexer->current_char>0){lexer->token->size++;}
}
void lexer_check_identifier(Lexer* lexer){
    for(int i = 0;i < KEYWORD_COUNT;i++){
        if(strcmp(lexer->token->value.string,keywords[i].name) == 0){
            lexer->token->type = keywords[i].type;
            break;
        }
    }
    lexer_reset_token(lexer);
}
