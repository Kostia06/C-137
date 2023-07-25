#include "../private.h"

// Start of an integer
void lexer_new_integer(Lexer* lexer){
    lexer->node->type = INTEGER;
    lexer->node->index = lexer->index;
    lexer->node->size = 1;
    lexer->node->value.string = mem_init(lexer->compiler->memory,sizeof(char)*2);
    lexer->node->value.string[0] = lexer->current_char;
    lexer_advance(lexer);
}
// When the current character is a dot, it means that the integer is a float
void lexer_add_dot(Lexer* lexer){
    lexer->node->type = FLOAT;
    lexer_add_char(lexer);        
}
// Error that tells that dot cannot be repeated twice in a float or in the beginning of a float
void lexer_error_decimal(Lexer* lexer){
    char* message = SYNC((char*[]){"Invalid decimal",NULL});
    error_single_init(lexer->compiler->error,SYNTAX_ERROR,lexer->index,lexer->index+1,message);
    lexer_advance(lexer);
}
