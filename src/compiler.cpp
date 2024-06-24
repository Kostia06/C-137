#include "../include/compiler.hpp"

Compiler::Compiler(string source){
    this->source = source;
    root = new Node(NodeType::ROOT, "");
    lexer = new Lexer(this->root, this->source);
    lexer->lex();
}
