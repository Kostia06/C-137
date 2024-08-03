#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "node.hpp"
#include "lexer.hpp"
#include "utils.hpp"
#include "error.hpp"

using namespace std;

class Compiler: private Error{
    private:
        string source;
        Node* root;
        Lexer* lexer;
    public:
        Compiler(string source);
        ~Compiler();

};
