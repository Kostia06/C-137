#ifndef LEXER_H
#define LEXER_H

#include "../utils/include.h"
#include "../hash/include.h"
#include "../file/include.h"
#include "../config/include.h"

void lex(Lexer* lexer);
Lexer* new_lexer(Config* config);


#endif