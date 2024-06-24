#include "../include/node.hpp"


Node::Node(NodeType type, string value){
    this->type = type;
    this->value = value;
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

// compare the current tree with another tree even if other tree has more nodes.
bool Node::compare(const Node* other){
    if(type != other->type) return false; 
    if(left && !left->compare(other->left)) return false;
    if(right && !right->compare(other->right)) return false;
    return true;
}
