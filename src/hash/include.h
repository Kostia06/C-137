#ifndef HASH_H
#define HASH_H

#include "../utils/include.h"


typedef struct{
    int type,* owners,size;
    void* value;
}Memory;
typedef struct{
    int type;
    void* value;
}HashValue;
typedef struct{
    HashValue* table[MAX_HASH_SIZE][MAX_HASH_SIZE];
}HashTable;

unsigned int hash_id(char *str);
int hash_find(HashTable* table,char* scope,int name);
void hash_add_value(HashTable* table,int scope,int name,void* value);



#endif
