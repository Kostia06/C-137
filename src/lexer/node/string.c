#include "../private.h"

// Start of a string
void lexer_new_string(Lexer* lexer){
    char c = lexer->current_char;
    lexer->node->type = c == '`' ? B_STRING : c == '"' ? D_STRING : S_STRING;
    lexer->node->value.string = mem_init(lexer->memory,sizeof(char)*2);
    lexer->node->index = lexer->index;
    lexer->node->size = 0;
    lexer_advance(lexer);    
}
// Add a character to the signle string
void lexer_single_string_add_char(Lexer* lexer){
    if(lexer->current_char == '\''){
        char c = lexer->node->value.string[0];
        lexer->node->value.integer = c;
        lexer->node->type = INTEGER;
        lexer_reset_node(lexer);
        lexer_advance(lexer);
        return;
    }
    if(lexer->current_char == '\\'){
        lexer_add_char(lexer);
        return;
    }
    if(lexer->node->size == 1){
        char* message = SYNC((char*[]){"Invalid single string",NULL});
        error_single_init(lexer->error,SYNTAX_ERROR,lexer->index,lexer->index+1,message);
    }
    lexer_add_char(lexer);
}
// end of the string
void lexer_string_end(Lexer* lexer){
    lexer->node->type = STRING;
    lexer_reset_node(lexer);
    lexer_advance(lexer);
}
