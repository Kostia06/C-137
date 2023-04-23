#ifndef PARSER_H
#define PARSER_H

#include "../utils/include.h"

Command** parse(Token** tokens,size_t size,char* scope,size_t* return_size);


#endif

