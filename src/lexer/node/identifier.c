#include "../private.h"

// Translates the escape sequence to the character
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
// Start of an identifier
void lexer_new_identifier(Lexer* lexer){
    lexer->node->type = IDENTIFIER;   
    lexer->node->index = lexer->index;
    lexer->node->size = 1;
    lexer->node->value.string = mem_init(lexer->compiler->memory,sizeof(char)*2);
    lexer->node->value.string[0] = lexer->current_char;
    lexer_advance(lexer);
}
// Add the current chacacter to the node and also checking the current character is part of the sequence
void lexer_add_char(Lexer* lexer){
    char c = lexer->current_char;
    if(lexer->current_char == '\\'){c = lexer_escape_sequence(lexer);}
    else{lexer_advance(lexer);}
    lexer->node->value.string = realloc(
        lexer->node->value.string,
        sizeof(char)*(strlen(lexer->node->value.string)+2)
    );
    lexer->node->value.string[strlen(lexer->node->value.string)] = c;
    if(lexer->current_char>0){lexer->node->size++;}
}
// Check if the current node is a keyword from the array "keywords"
void lexer_check_identifier(Lexer* lexer){
    for(int i = 0;i < KEYWORD_COUNT;i++){
        if(!strcmp(lexer->node->value.string,keywords[i].name)){
            lexer->node->type = keywords[i].type;
            mem_free(lexer->compiler->memory,lexer->node->value.string);
            lexer->node->value.string = NULL;
            break;
        }
    }
    lexer_reset_node(lexer);
}
