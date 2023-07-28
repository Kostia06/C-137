#include "../private.h"

void lexer_new_macro(Lexer* lexer){
    lexer->node->type = MACRO;   
    lexer->node->index = lexer->index;
    lexer->node->size = 1;
    lexer->node->value.string = mem_init(lexer->compiler->memory,sizeof(char)*1);
    lexer_advance(lexer);
}
void lexer_check_macro(Lexer* lexer){
    for(int i = 0;i < MACRO_COUNT;i++){
        if(strcmp(lexer->node->value.string,macros[i].name) == 0){
            lexer->node->type = macros[i].type;
            mem_free(lexer->compiler->memory,lexer->node->value.string);
            lexer_reset_node(lexer);
            return;
        }
    }
    char* message = SYNC((char*[]){"Unknown macro \"",lexer->node->value.string,"\"",NULL});
    error_single_init(
        lexer->compiler->error,
        SYNTAX_ERROR,
        lexer->node->index,
        lexer->node->index+1,
        message
    );
    lexer_reset_node(lexer);
}
