#ifndef PARSER_MACRO_H
#define PARSER_MACRO_H

#include "../../utils/include.h"
#include "../../hash/include.h"

Node** create_macro(Node** nodes,size_t size,char* scope,size_t* return_size);

#endif
