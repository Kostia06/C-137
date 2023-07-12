#ifndef PARSER_H
#define PARSER_H

#include "../utils/include.h"

Vector* parse(ErrorGroup* error,MemoryGroup* token_memory,MemoryGroup* memory,Vector* tokens,int type, char* scope);


#endif  
