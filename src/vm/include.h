#ifndef VM_H
#define VM_H

#include "../utils/include.h"
#include "../stack/include.h"
#include "../hash/include.h"

int vm(HashTable* table, Stack* stack, Token** tokens, size_t size, char* scope);


#endif
