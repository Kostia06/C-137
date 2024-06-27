#pragma once

#include <iostream>
#include <string>

#include "nodeType.hpp"

using namespace std;

class Node{
    private:
    public:
        string valueStr;
        int valueInt;

        NodeType type;
        Node* left, *right;

        Node(NodeType type = EMPTY, string str = "");

        void remove(Node* node);

        Node* predecessor();
        Node* successor();

        bool compare(const Node* other);
        void print(string prefix = "", bool isTail = true);
};
