#ifndef LEXER_H
#define LEXER_H

#include "../utils/include.h"

Vector* new_lexer(ErrorGroup* error,MemoryGroup* memory,char* text, char* scope);

#endif
