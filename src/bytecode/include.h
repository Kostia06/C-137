#ifndef BYTECODE_H
#define BYTECODE_H

#include "../utils/include.h"
#include "../hash/include.h"

Bytecode** build_bytecodes(HashTable* table,Token** tokens,size_t size,char* scope,size_t* return_size);


#endif

