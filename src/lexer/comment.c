#include "private.h"

void lexer_comment(Lexer* lexer){
    lexer_advance(lexer);
    lexer->token->type = T_COMMENT;
}
void lexer_end_comment(Lexer* lexer){lexer->token->type = T_EMPTY;}

