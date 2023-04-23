#ifndef HASH_H
#define HASH_H

#include "../utils/include.h"

typedef struct{
    int type;
    void* value;
}Memory;

typedef struct{
    char** scopes;
    int size;
    Memory* table[MAX_HASH_SIZE][MAX_HASH_SIZE];
}HashTable;

unsigned int hash_id(char *str);
Memory* new_memory(int type,void* value);
Memory* get_memory(HashTable* table,int scope,int name);
void set_memory(HashTable* table,int scope,int name,Memory* memory);
int hash_find(HashTable* table,char* scope,int name);

#endif
