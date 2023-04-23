#include "include.h"

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
Memory* get_memory(HashTable* table,int scope,int name){
    return table->table[scope][name];
}
void set_memory(HashTable* table,int scope,int name,Memory* memory){
    table->table[scope][name] = memory;
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

