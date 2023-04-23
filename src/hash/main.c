#include "include.h"

HashTable* new_hash_table(){
    HashTable* table = malloc(sizeof(HashTable));
    table->size = 0;
    hash_create_scope(table,"PUBLIC");
    return table;
}

unsigned int hash_id(char* str){
    unsigned int value = 0;
    while (*str) {value = value * 31 + *str++;}
    return value % MAX_HASH_SIZE;
}   
Memory* new_memory(int type,void* value){
    Memory* memory = malloc(sizeof(Memory));
    memory->type = type;
    memory->value = value;
    return memory;
}
<<<<<<< HEAD
Memory* get_memory(HashTable* table,int scope,int name){
    return table->table[scope][name];
}
void set_memory(HashTable* table,int scope,int name,Memory* memory){
    table->table[scope][name] = memory;
=======

void hash_create_scope(HashTable* table,char* scope){
    HashScope* hash_scope = malloc(sizeof(HashScope));
    int index = hash_id(scope);
    hash_scope->size = 0;
    table->entries[index] = hash_scope;
    table->ids[table->size] = index;
    table->names[table->size++] = scope;
}

void hash_create_entry(HashTable* table,int scope,int name,Memory* memory){
    HashScope* hash_scope = table->entries[scope];
    HashEntry* hash_entry = malloc(sizeof(HashEntry));
    hash_entry->value = memory;
    hash_scope->entries[name] = hash_entry;
    hash_scope->ids[hash_scope->size++] = name;
}
Memory* hash_create_memory(void* value,int type){
    Memory* memory = malloc(sizeof(Memory));
    memory->value = value;
    memory->type = type;
    return memory;
}
Memory* hash_get_memory(HashTable* table,int scope,int name){
    HashScope* hash_scope = table->entries[scope];
    HashEntry* hash_entry = hash_scope->entries[name];
    return hash_entry->value;
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
}
void hash_change_memory(HashTable* table,void* value,int scope,int name){
    HashScope* hash_scope = table->entries[scope];
    HashEntry* hash_entry = hash_scope->entries[name];
    hash_entry->value = value;
}

int hash_find(HashTable* table,char* scope,int name){
    size_t size = 0;
    char** scopes = ALL_SCOPES(scope,&size);
    for(int i = 0;i<(int)size;i++){
        int scope_id = hash_id(scopes[i]);
        if(table->table[scope_id][name] != NULL){return scope_id;}
    }
    return -1;
}
<<<<<<< HEAD
=======

>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
