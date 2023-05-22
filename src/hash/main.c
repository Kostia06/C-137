#include "include.h"

unsigned int hash_id(char* str){
    unsigned int value = 0;
    while (*str) {value = value * 31 + *str++;}
    return value % MAX_HASH_SIZE;
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
void hash_add_value(HashTable* table,int scope,int name,void* value){
    table->table[scope][name] = value; 
}
