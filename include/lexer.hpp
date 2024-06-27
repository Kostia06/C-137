#pragma once

#include <fstream>
#include <vector>
#import <map>

#include "node.hpp"

using namespace std;

class Lexer{
    private:
        vector<Node*> nodes;
        Node* root, *currentNode;
        string source, content;
        int pos = 0, currentLine = 1, contentSize = 0;

        vector<char> symbols = {
            '+', '-', '*', '/', '%', '^',
            '(', ')', '[', ']',
            '=', '.', ',', ':', ';',
            '>', '<', '!', '&', '|'
        };

        map<string, NodeType> keywordMap = {
            {"if", IF}, {"else", ELSE}, {"loop", LOOP},
            {"return", RETURN}, {"break", BREAK}, {"continue", CONTINUE},
            {"function", FUNCTION}, {"enum", ENUM}, {"obj", OBJ}
        };

        int symbolLength = 2;
        map<string, NodeType> symbolMap = {
            {"+", PLUS}, {"-", MINUS}, {"*", MULTIPLY}, {"/", DIVIDE}, {"%", MODULO}, {"^", POWER},
            {"(", ARGUMENT_START}, {")", ARGUMENT_END}, {"[", ARRAY_START}, {"]", ARRAY_END},
            {"=", ASSIGNMENT}, {".", DOT}, {",", COMMA}, {":", COLON}, {";", SEMICOLON},
            {"==", EQUAL}, {"!=", NOT_EQUAL}, {">", GREATER}, {"<", LESS}, {">=", GREATER_EQUAL}, {"<=", LESS_EQUAL},
            {"&&", AND}, {"||", OR}, {"!", NOT}
        };

        string read();
        void addNode();
        void handleIndent();
        void handleNumber();
        void handleIdentifier();
        void handleString();
        void handleSymbol();
    public:
        Lexer(Node* root, string source);
        
        void lex();
};
