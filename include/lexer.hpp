#pragma once

#include "node.hpp"

class Lexer{
    private:
        Node* root;
        string source;
        string text; 
        int pos;
    public:
        Lexer(Node* root, string source);
        
        void lex();
};
