#pragma once

#include "../utils/include.hpp"



class Node{
    private:
    public:
        Type type;
        std::string value;
        std::vector<Node*> children;
        size_t index, size;

        Node(Type type, std::string value, size_t index, size_t size);
        ~Node();

        void print(int level = 0);
};
