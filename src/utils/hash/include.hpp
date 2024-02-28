#pragma once

#include <iostream>
#include <vector>

class HASH_NODE;

class HASH_NODE{
    private:
        size_t size;
    public:
        void* data;
        std::vector<HASH_NODE*> nodes;
    
        HASH_NODE(size_t size);
        void add(std::vector<int> keys, void * data);
        void* get(std::vector<int> keys);
};
