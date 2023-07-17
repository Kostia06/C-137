#include "../include.h"

// initialize a hash table
Hash* hash_init(MemoryGroup* memory){
    Hash* hash = mem_init(memory,sizeof(Hash));
    hash->value_keys = vector_init();
    hash->memory = memory;
    return hash;
}
// turns a string into a integer key, so that it can access arrays
int hash_id(char* key){
    unsigned int hash = 0;
    for(int i = 0; key[i] != '\0'; i++){hash = 31 * hash + key[i];}
    return hash % HASH_SIZE;
}
// add a value to the hash table
void hash_add(Hash* hash, int key,int type, char* name, void* value){
    HashKey* hash_key = mem_init(hash->memory,sizeof(HashKey));
    hash_key->value = value;
    hash_key->type = type;
    hash->table[key] = hash_key;
    // add the name to the value_keys array
    char* hash_name = mem_init(hash->memory,sizeof(char)*strlen(name));
    strcpy(hash_name,name);
    vector_add(hash->value_keys,hash_name);
}
// get a value from the hash table
HashKey* hash_get(Hash* hash,int key){return hash->table[key];}
// check if a value exists in the hash table
int hash_find(Hash* hash,int key){return hash->table[key]!=NULL;}
// clear the hash
Vector* hash_get_vector(Hash* hash){
    Vector* vector = vector_init();
    for(int i=0;i<(int)hash->value_keys->size;i++){
        char* key = vector_get(hash->value_keys,i);
        vector_add(vector,hash_get(hash,hash_id(key)));
    }
    return vector;
}
