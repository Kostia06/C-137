#ifndef HASH_H
#define HASH_H

#include "../utils/include.h"

HashTable *new_hash_table();
unsigned int hash_id(char *str);

void hash_create_scope(HashTable *table, char *scope);

void hash_create_entry(HashTable *table, int scope, int name, Memory *memory);

Memory *hash_create_memory(void* value,int type);   
Memory *hash_get_memory(HashTable* table, int scope, int name);
void hash_change_memory(HashTable* table, void *value, int scope, int name);

int hash_find(HashTable *table, char *scope, int name);

#endif
