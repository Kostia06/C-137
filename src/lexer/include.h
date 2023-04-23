#ifndef LEXER_H
#define LEXER_H

#include "../utils/include.h"
#include "../file/include.h"

Token** format(Token** tokens, char* scope, size_t size,size_t* return_size);
Lexer* new_lexer(CompilerOptions* options);

#endif
