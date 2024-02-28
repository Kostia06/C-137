#include "include.hpp"


HASH_NODE::HASH_NODE(size_t size){
    nodes = std::vector<HASH_NODE*>(size, nullptr);
    this->size = size;
}

void HASH_NODE::add(std::vector<int> keys, void* data){
    HASH_NODE* current = this;
    int key = keys[0];
    keys.erase(keys.begin());
    if(current->nodes[key] == NULL){ 
        current->nodes[key] = new HASH_NODE(size); 
        current = current->nodes[key];
    }
    else{ current = current->nodes[key]; }
    if(keys.size() == 0){ current->data = data; }
    else{ current->add(keys, data); }
}

void* HASH_NODE::get(std::vector<int> keys){
    HASH_NODE* current = this;
    int key = keys[0];
    keys.erase(keys.begin());
    if(current->nodes[key] == nullptr){ 
        return NULL;
    }
    else{ current = current->nodes[key]; }
    if(keys.size() == 0){ return current->data; }
    return current->get(keys);
}
