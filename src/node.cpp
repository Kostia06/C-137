#include "../include/node.hpp"


Node::Node(NodeType type, string str){
    this->type = type;
    this->valueStr = str;
    this->valueInt = 0;
    this->left = nullptr;
    this->right = nullptr;
}

Node* Node::predecessor(){
    Node* node = left;
    while(node && node->right) 
        node = node->right;
    return node;
}

Node* Node::successor(){
    Node* node = right;
    while(node && node->left) 
        node = node->left; 
    return node;
}

bool Node::compare(const Node* other){
    if(type != other->type) 
        return false; 
    if(left && !left->compare(other->left)) 
        return false;
    if(right && !right->compare(other->right)) 
        return false;
    return true;
}

void Node::print(string prefix, bool isTail){
    cout << prefix << (isTail ? "└── " : "├── ");
    if(valueStr == "") 
        cout << valueInt << endl;
    else
        cout << valueStr << endl;
    if(left) 
        left->print(prefix + (isTail ? "    " : "│   "), !right);
    if(right) 
        right->print(prefix + (isTail ? "    " : "│   "), true);
}
