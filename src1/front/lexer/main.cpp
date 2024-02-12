#include "include.hpp"

#include "../../compiler/include.hpp"
/*
    1. handle spaces and add it to the New Line
    2. handle symbols + comments
    3. handle strings + chars
    4. handle numbers + floats
    5. handle keywords + ids
    6. handle ops
    7. handle addresses
*/

Lexer::Lexer(Compiler* compiler): compiler(compiler){
    index = 0;
    while(index < compiler->current_file_content.size()){
        if(handle_spaces()){ continue; }
        else if(handle_symbols()){ continue; }
        else if(handle_strings()){ continue; }
        else if(handle_numbers()){ continue; }
        else if(handle_keywords()){ continue; }
        else if(handle_addresses()){ continue; }
        get_next_char();
    }
}
