#include "include.hpp"

#include "../../compiler/include.hpp"

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
