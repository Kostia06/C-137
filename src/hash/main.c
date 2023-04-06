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
}
void hash_change_memory(HashTable* table,void* value,int scope,int name){
    HashScope* hash_scope = table->entries[scope];
    HashEntry* hash_entry = hash_scope->entries[name];
    hash_entry->value = value;
}

int hash_find(HashTable* table,char* scope,int name){
    for(int i=table->size-1;i>0;i--){
        if(strstr(scope,table->names[i])!=NULL || !strcmp(table->names[i],"PUBLIC")){
            HashScope* hash_scope = table->entries[table->ids[i]];
            if(hash_scope->entries[name] != NULL){
                char* return_scope = strstr(scope,table->names[i])!=NULL ? table->names[i] : "PUBLIC";
                return hash_id(return_scope);
            }
        }
    }
    return -1;
}

