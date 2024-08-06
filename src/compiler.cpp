#include "../include/compiler.hpp"

Compiler::Compiler(int argc, char* argv[]){
    root = new Node(NodeType::ROOT, "");
    argumentParser = new Argument();
    if(argumentParser->error())
        return;
    /*
    lexer = new Lexer(this->root, this->source);
    lexer->lex();
    if(lexer->error())
        return;
    */
}
