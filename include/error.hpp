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

#define ERROR_SYNTAX "syntax error"
#define ERROR_UNKNOWN_CHARACTER "unknown character"
#define ERROR_UNKNOWN_SYMBOL "unknown symbol"
#define ERROR_UNTERMINATED "unterminated string"
#define ERROR_INVALID_FLOAT "invalid float"
#define ERROR_FILE_NOT_FOUND "file not found"

enum ErrorType{
    WARNING, 
    ERROR
};

class Error{
    private:
        bool hasError = false;
        vector<string> errors;

        string findError(string fileName, int lineNum, int columnNum, int size = 1);
    public:
        void logInFileError(string fileName, string message, string error, int lineNum, int columnNum, int size = 1, ErrorType type = ERROR);
        void logInFileError(string fileName, string message, string error, Node* node, ErrorType type = ERROR);
        void logError(string message, string error, ErrorType type = ERROR);
        bool error();
};

