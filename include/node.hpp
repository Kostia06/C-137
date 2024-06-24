#pragma once

#include <iostream>
#include <string>

#include "nodeType.hpp"

using namespace std;

class Node{
    private:
    public:
        string value;
        NodeType type;
        Node* left, *right;

        Node(NodeType type = EMPTY, string value = "");

        void remove(Node* node);

        Node* predecessor();
        Node* successor();

        bool compare(const Node* other);
};
