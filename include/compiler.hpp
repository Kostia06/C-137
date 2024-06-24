#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "node.hpp"
#include "lexer.hpp"

using namespace std;

class Compiler{
    private:
        string source;
        Node* root;
        Lexer* lexer;
    public:
        Compiler(string source);
        ~Compiler();

};
