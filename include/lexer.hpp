#pragma once

#include <fstream>
#include <vector>
#include <map>

#include "node.hpp"
#include "utils.hpp"
#include "error.hpp"

using namespace std;

class Lexer: public Error{
    private:
        Node* root, *currentNode;
        string source, content;
        int pos = 0, line = 1, column = 1, contentSize = 0;

        vector<char> symbols = {
            '+', '-', '*', '/', '%', '^',
            '(', ')', '[', ']',
            '=', '.', ',', ':', ';',
            '>', '<', '!', '&', '|',
        };

        map<string, NodeType> keywordMap = {
            {"if", IF}, {"else", ELSE}, {"loop", LOOP},
            {"return", RETURN}, {"break", BREAK}, {"continue", CONTINUE},
            {"function", FUNCTION}, {"enum", ENUM}, {"obj", OBJ},
            {"_", UNDERSCORE},
        };

        int symbolLength = 2;
        map<string, NodeType> symbolMap = {
            {"+", PLUS}, {"-", MINUS}, 
            {"*", MULTIPLY}, {"/", DIVIDE}, 
            {"%", MODULO}, {"^", POWER},
            {"==", EQUAL}, {"!=", NOT_EQUAL}, 
            {">", GREATER}, {"<", LESS}, 
            {">=", GREATER_EQUAL}, {"<=", LESS_EQUAL},
            {"&&", AND}, {"||", OR}, {"!", NOT},
            {"(", ARGUMENT_START}, {")", ARGUMENT_END}, 
            {"[", ARRAY_START}, {"]", ARRAY_END},
            {"|>", PIPE}, {"=", ASSIGNMENT}, 
            {".", DOT}, {",", COMMA}, 
            {":", COLON}, {";", SEMICOLON},
        };

        void next();
        void prev();
        void addNode(Node* node);

        void handleIndent();
        void handleNewLine();
        void handleNumber();
        void handleIdentifier();
        void handleString();
        void handleSymbol();
        void handleUknown();

    public:
        Lexer(Node* root, string source);        
        void lex();
};
