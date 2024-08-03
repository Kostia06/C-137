#pragma once

#include <iostream>
#include <string>

using namespace std;

enum NodeType{
    EMPTY,
    ROOT,
        // symbols
        ASSIGNMENT, DOT, COMMA, COLON, SEMICOLON,
        NEWLINE, INDENT, UNDERSCORE,
        // operators
        PLUS, MINUS, MULTIPLY, DIVIDE, MODULO, POWER, PIPE,
        EQUAL, NOT_EQUAL, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL,
        AND, OR, NOT,
        ARGUMENT_START, ARGUMENT_END,
        ARRAY_START, ARRAY_END,
        // Keywords
        IF, ELSE, LOOP, 
        RETURN, BREAK, CONTINUE, 
        FUNCTION, ENUM, OBJ,
        // Data types
        IDENTIFIER,
        NUMBER, FLOAT,
        STRING, ADDRESS,
    END,
};

const string stringNodeType[END + 1] = {
    "EMPTY",
    "ROOT",
        // symbols
        "ASSIGNMENT", "DOT", "COMMA", "COLON", "SEMICOLON",
        "NEWLINE", "INDENT", "UNDERSCORE",
        // operators
        "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "MODULO", "POWER", "PIPE",
        "EQUAL", "NOT_EQUAL", "GREATER", "LESS", "GREATER_EQUAL", "LESS_EQUAL",
        "AND", "OR", "NOT",
        "ARGUMENT_START", "ARGUMENT_END",
        "ARRAY_START", "ARRAY_END",
        // Keywords
        "IF", "ELSE", "LOOP",
        "RETURN", "BREAK", "CONTINUE",
        "FUNCTION", "ENUM", "OBJ",
        // Data types
        "IDENTIFIER",
        "NUMBER", "FLOAT",
        "STRING", "ADDRESS",
    "END",
};


class Node{
    private:
    public:
        string valueStr;
        int valueInt;

        int line, column, size;

        NodeType type;
        Node* left, *right;

        Node(NodeType type = EMPTY, string str = "");

        void remove(Node* node);

        Node* predecessor();
        Node* successor();

        bool compare(const Node* other);
        void print(string prefix = "", bool isTail = true);
};
