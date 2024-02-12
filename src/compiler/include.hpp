#pragma once

#include "../utils/include.hpp"
#include "../node/include.hpp"
#include "../error/include.hpp"

#include "../front/lexer/include.hpp"
#include "../front/parser/include.hpp"

class Compiler{
    private:
        void run(std::string file_name);

        Lexer* lexer;
        Parser* parser;
    public:
        ErrorHandler* error_handler;
        std::vector<Node*> nodes;
        std::vector<Keyword> signs;
        std::vector<Keyword> keywords;
        std::string current_file_name;
        std::string current_file_content;

        Compiler(char** argv, int argc);
        ~Compiler();   
    
        
};
