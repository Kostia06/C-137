#ifndef HASH_H
#define HASH_H

#include "../utils/include.h"

<<<<<<< HEAD
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
=======
HashTable *new_hash_table();
unsigned int hash_id(char *str);

void hash_create_scope(HashTable *table, char *scope);

void hash_create_entry(HashTable *table, int scope, int name, Memory *memory);

Memory *hash_create_memory(void* value,int type);   
Memory *hash_get_memory(HashTable* table, int scope, int name);
void hash_change_memory(HashTable* table, void *value, int scope, int name);

int hash_find(HashTable *table, char *scope, int name);
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617

#endif
