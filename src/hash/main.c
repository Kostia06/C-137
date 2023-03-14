#include "include.h"

unsigned int hash_id(char* str){
    unsigned int value = 0;
    while (*str) {
        value = value * 31 + *str++;
    }
    return value % MAX_HASH_SIZE;
}

void hash_scope_init(HashTable* table,char* scope){
    HashScope* hash_scope = malloc(sizeof(HashScope));
    int index = hash_id(scope);
    hash_scope->size = 0;
    ERROR(table->entries[index]!=NULL,0,(char*[]){"Hash table \"scope\" collision with \"",scope,"\"",NULL},__func__,scope);
    table->entries[index] = hash_scope;
    table->ids[table->size] = index;
    table->names[table->size++] = scope;
}

void hash_entry_init(HashTable* table,char* scope,int name,int type){
    int index = hash_id(scope);
    HashScope* hash_scope = table->entries[index];
    ERROR(hash_scope==NULL,0,(char*[]){"Hash table \"scope\" not found, \"",scope,"\"",NULL},__func__,scope);
    HashEntry* hash_entry = malloc(sizeof(HashEntry));
    hash_entry->type = type;
    ERROR(hash_scope->entries[name]!=NULL,0,(char*[]){"Hash table \"entry\" collision with \"",scope,"\"",NULL},__func__,scope);
    hash_scope->entries[name] = hash_entry;
    hash_scope->ids[hash_scope->size++] = name;
}
int hash_get_entry_type(HashTable* table,int scope,int name){
    HashScope* hash_scope = table->entries[scope];
    HashEntry* hash_entry = hash_scope->entries[name];
    return hash_entry->type;
}

void hash_create_memory(HashTable* table,int scope,int name,void *value){
    HashScope* hash_scope = table->entries[scope];
    HashEntry* hash_entry = hash_scope->entries[name];
    hash_entry->value = value;
}
void* hash_get_memory(HashTable* table,int scope,int name){
    HashScope* hash_scope = table->entries[scope];
    HashEntry* hash_entry = hash_scope->entries[name];
    return hash_entry->value;
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

void hash_print(HashTable* table){
    // for(int i=0;i<table->size;i++){
    //     int id_scope = table->ids[i];
    //     HashScope* hash_scope = table->entries[id_scope];
    //     printf("%d:Scope: %s\t%d\n",i,table->names[i],table->ids[i]);
    //     for(int j=0;j<hash_scope->size;j++){
    //         int id_entry = hash_scope->ids[j];
    //         HashEntry* hash_entry = hash_scope->entries[id_entry];
    //         int type = hash_entry->type;
    //         char* stars = malloc(sizeof(char)*type.size);
    //         for(int k=0;k<type.size;k++) stars[k] = '*';
    //         printf("\t%d:Entry: %d\tType: %s%s\n",j,id_entry,TYPE(type),stars);
    //     }
    // }
}