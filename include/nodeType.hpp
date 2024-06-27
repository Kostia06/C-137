#pragma once

enum NodeType{
    EMPTY,
    ROOT,
    // Symbols
        // operators
        OPERATOR_START,
            PLUS, MINUS, MULTIPLY, DIVIDE, MODULO, POWER,
        OPERATOR_END,
        // parentheses
        ARGUMENT_START, ARGUMENT_END,
        ARRAY_START, ARRAY_END,
        // random
        ASSIGNMENT, DOT, COMMA, COLON, SEMICOLON,
        INDENT,
        // comparison + logical
        COMPARISON_START,
            EQUAL, NOT_EQUAL, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL,
            AND, OR, NOT,
        COMPARISON_END,
    // Keywords
        // in body
        BODY_START, 
            IF, ELSE, LOOP, 
            RETURN, BREAK, CONTINUE, 
        BODY_END,
        // in declaration
        DECLARATION_START,
            FUNCTION, ENUM, OBJ,
        DECLARATION_END,
    // Data types
        DATA_START,
            IDENTIFIER,
            NUMBER, FLOAT,
            STRING,
        DATA_END,
    END,
};


