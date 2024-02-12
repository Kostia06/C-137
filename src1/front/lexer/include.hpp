#pragma once

#include "../../utils/include.hpp"
#include "../../node/include.hpp"
#include "../../error/include.hpp"

// shadow declaring Compiler
class Compiler;

class Lexer{
    private:
        Compiler* compiler;

        size_t index = 0;
        char c;

        bool get_next_char();
        bool get_prev_char();

        bool handle_spaces();
        bool handle_symbols();
        bool handle_strings();
        bool handle_numbers();
        bool handle_keywords();
        bool handle_addresses();

    public:
        Lexer(Compiler* compiler);
};
