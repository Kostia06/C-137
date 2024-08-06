#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "node.hpp"
#include "lexer.hpp"
#include "utils.hpp"
#include "error.hpp"
#include "argument.hpp"

using namespace std;

class Compiler: private Error{
    private:
        Argument* argumentParser;
        Lexer* lexer;
        Node* root;
    public:
        Compiler(int argc, char* argv[]);
        ~Compiler();

};
