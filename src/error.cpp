#include "../include/error.hpp"


string Error::findError(string fileName, int lineNum, int columnNum, int size){
    // gettig the line where the error is
    string line = split(readFile(fileName), "\n")[lineNum - 1]; 
    // stringifying the line and column
    string lineNumStr = to_string(lineNum);
    string columnNumStr = to_string(columnNum);
    // creating an empty string with spaces
    string empty = string(lineNumStr.size() + 1, ' ');
    // getting the max size of the error, so it doesn't go out of bounds 
    int maxSize = columnNum + size > line.size() ? line.size() - columnNum + 1: size;
    maxSize = maxSize == 0 ? 1: maxSize;
    return "" 
        + empty +BORDER_TOP_LEFT + " " + fileName + ":" + lineNumStr + ":" + columnNumStr + "\n"
        + lineNumStr + " " + BORDER_VERTICAL + "\t"+ line + "\n"
        + empty + BORDER_VERTICAL + "\t" + string(columnNum-1, ' ') + string(maxSize, '^') + "\n"
    ;
}

void Error::logInFileError(string fileName, string message, string error, int lineNum, int columnNum, int size, ErrorType type){
    columnNum--; // 0 based index
    if(type == ERROR)
        hasError = true;
    // specific error type
    string typeStr = type == WARNING ? "warning" : "error";
    // creating the error message
    string result = "" 
        + typeStr + ": " + error + "\n"
        + findError(fileName, lineNum, columnNum, size)
    ;
    errors.push_back(result);
}

void Error::logInFileError(string fileName, string message, string error, Node* node, ErrorType type){
    logInFileError(fileName, message, error, node->line, node->column, node->size, type);
}

void Error::logError(string message, string error, ErrorType type){
    if(type == ERROR)
        hasError = true;
    // specific error type
    string typeStr = type == WARNING ? "warning" : "error";
    string result = typeStr + ": " + error + "\n" + message;
    errors.push_back(result);
}

bool Error::error(){
    for(string e: errors)
        cout << e << endl;
    return hasError;
}
