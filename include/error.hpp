#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "utils.hpp"
#include "node.hpp"

using namespace std;

#define BORDER_TOP_LEFT string("┌")
#define BORDER_TOP_RIGHT string("┐")
#define BORDER_BOTTOM_LEFT string("└")
#define BORDER_BOTTOM_RIGHT string("┘")
#define BORDER_HORIZONTAL string("─")
#define BORDER_VERTICAL string("│")

#define ERROR_SYNTAX "SyntaxError"
#define ERROR_UNKNOWN_CHARACTER "unknown character"
#define ERROR_UNKNOWN_SYMBOL "unknown symbol"
#define ERROR_UNTERMINATED "unterminated string"
#define ERROR_INVALID_FLOAT "invalid float"

#define ERROR_FILE_NOT_FOUND "file not found"

#define ERROR_ARGUMENT "ArguemntError"
#define WARNING_ARGUMENT "ArgumentWarning"
#define WARNING_UNEXPECTED_ARGUMENT "unexpected argument"


#define ERROR_INVALID_ARGUMENT(value) string("invalid argument <") + value + string(">")
#define ERROR_ARGUMENT_ALREADY_EXISTS(value) string("argument <") + value + string("> already exists")
#define ERROR_REQUIRED_ARGUMENT_MISSING(value) string("required argument for <") + value + string(">is missing")
#define ERROR_INVALID_ARGUMENT_VALUE(value) string("invalid argument value <") + value + string(">")
#define ERROR_ELSE_ARGUMENT_ALREADY_EXISTS "ELSE argument already exists"

enum ErrorType{
    WARNING, 
    ERROR
};

class Error{
    private:
        bool hasError = false;
        string fileName = "";
        string content = "";
        vector<string> errors;

        string findError(int lineNum, int columnNum, int size = 1);
    public:
        void setDetails(string fileName, string content);
        void logError(string message, string error, int lineNum, int columnNum, int size = 1, ErrorType type = ERROR);
        void logError(string message, string error, Node* node, ErrorType type = ERROR);
        bool error();
};

