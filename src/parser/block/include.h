#ifndef PARSER_BLOCK_H
#define PARSER_BLOCK_H

#include "../../utils/include.h"
#include "../../hash/include.h"

NodeBlock** create_blocks(Node** nodes,size_t size,char* scope,size_t* return_size);

#endif
