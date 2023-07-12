#include "../private.h"
/**/
// Start of a comment
void lexer_comment_advance(Lexer* lexer){
    if(lexer->current_char == '*'){
        if(lexer_peek(lexer) == '/'){
            lexer_advance(lexer);
            lexer_advance(lexer);
            lexer_end_comment(lexer);
            return;
        }
    }
    lexer_advance(lexer);
}
// Set node to EMPTY, indicating that the comment has ended
void lexer_end_comment(Lexer* lexer){
    lexer->node->type = EMPTY;
}

