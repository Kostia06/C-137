#ifndef PARSER_H
#define PARSER_H

#include "../utils/include.h"
#include "../vector/include.h"

void parser_init();
Vector* parse(Vector* tokens,int type,char* scope);


#endif
