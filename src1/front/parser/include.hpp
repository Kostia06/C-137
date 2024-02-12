#pragma once

#include "../../utils/include.hpp"

// shadowing Compiler
class Compiler;

class Parser{
    private:
        Compiler* compiler;
    public:
        Parser(Compiler* compiler);

};
