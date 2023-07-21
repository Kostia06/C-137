#include "../private.h"

// Start of a new line
void lexer_new_line_init(Lexer* lexer){
    lexer->node->type = NEW_LINE;
    lexer->node->value.integer = 0;
    lexer->node->index = lexer->index;
    lexer->node->size = 1;
    lexer_advance(lexer);
}
// If newline is found in the string then add it to the node
void lexer_add_new_line(Lexer* lexer){
    lexer->node->value.string = realloc(
        lexer->node->value.string,
        sizeof(char)*(strlen(lexer->node->value.string)+2)
    );
    lexer->node->value.string[strlen(lexer->node->value.string)] = '\n';
    lexer_advance(lexer);
}
// If current char is a space the count it and advance so that when building blocks it will easy to put commands in blocks
void lexer_count_space(Lexer* lexer){
    lexer->node->value.integer++;
    lexer_advance(lexer);
}
