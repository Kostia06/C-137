#ifndef HASH_H
#define HASH_H

#include "../utils/include.h"

unsigned int hash_id(char* str);

void hash_scope_init(HashTable* table,char* scope);

void hash_entry_init(HashTable* table,char* scope,int name,int type);
int hash_get_entry_type(HashTable* table,int scope,int name);

void hash_create_memory(HashTable* table,int scope,int name,void *value);
void* hash_get_memory(HashTable* table,int scope,int name);

int hash_find(HashTable* table,char* scope,int name);

void hash_print(HashTable* table);

#endif