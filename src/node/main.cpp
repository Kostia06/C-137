#include "include.hpp"



Node::Node(Type type, std::string value, size_t index, size_t size){
    this->type = type;
    this->value = value;
    this->size = size;
    this->index = index;
}

Node::~Node(){
    for(int i = 0; i < children.size(); i++){ delete children[i]; }
}

void Node::print(int level){
    std::string str = "";
    for(int i = 0; i < level; i++){ str += "  "; }
    str += type_str();
    for(int i = 0; i < 25 - type_str().size(); i++){ str += " "; }
    str += value;
    std::cout << str << std::endl;
    for(int i = 0; i < children.size(); i++){ children[i]->print(level+1); }
}

std::string Node::type_str(){ return CType[type]; }
