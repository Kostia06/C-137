#include "include.h"
#include "private.h"

#define DEBUG_TOKEN     0

static int compare_keyword(const void* a,const void* b){
    Keyword* keyword_a = (Keyword*)a;
    Keyword* keyword_b = (Keyword*)b;
    return strlen(keyword_b->name) - strlen(keyword_a->name);
}


Token* new_empty_token(int line){
    Token* token = malloc(sizeof(Token));
    token->type = T_EMPTY;
    token->line = line;
    return token;
}
void lexer_reset_token(Lexer* lexer){
    vector_add(lexer->tokens,lexer->token);
    lexer->token = new_empty_token(lexer->line);
}
void lexer_advance(Lexer* lexer){
    lexer->index++;
    lexer->column++;
    lexer->current_char = lexer->text[lexer->index];
}


Vector* new_lexer(char* text, char* scope){
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->tokens = vector_init();
    lexer->token = new_empty_token(1);
    lexer->token_size = 0;
    lexer->line = 0;
    lexer->column = 0;
    lexer->index = 0;
    lexer->text = malloc(sizeof(char)*strlen(text)+2);
    lexer->text[0] = '\n';
    strcat(lexer->text,text);
    lexer->text_size = strlen(text)+1;
    lexer->scope = scope;
    lexer->current_char = lexer->text[lexer->index];
    qsort(keywords,KEYWORD_COUNT,sizeof(Keyword),compare_keyword);
    qsort(signs,SIGN_COUNT,sizeof(Keyword),compare_keyword);
    while(lexer->index < lexer->text_size){
        int type = 0;
        if(lexer->current_char < 0){type = LT_ALPHA;}
        else{type = characters[lexer->current_char];}
        lexer_function function = states[lexer->token->type][type];
        function(lexer);
    }
    #if DEBUG_TOKEN == 1
        for(int i = 0; i < lexer->tokens->size; i++){
            Token* token = vector_get(lexer->tokens,i);
            PRINT_TOKEN(token);
        }
    #endif
    {
        char* error[] = {"String not closed",NULL};
        int e_condition = lexer->token->type != T_NEW_LINE;
        ERROR(e_condition,lexer->token->line,lexer->token->column,1,error,lexer->scope);
    }
    lexer_reset_token(lexer);
    Vector* tokens = lexer->tokens;
    free(lexer->text);
    free(lexer); 
    return tokens;
}
