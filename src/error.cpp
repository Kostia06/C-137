#include "../include/error.hpp"

void Error::setDetails(string fileName, string content){
    this->fileName = fileName;
    this->content = content;
}

string Error::findError(int lineNum, int columnNum, int size){
    // gettig the line where the error is
    string line = split(content, "\n")[lineNum - 1]; 
    // stringifying the line and column
    string lineNumStr = to_string(lineNum);
    string columnNumStr = to_string(columnNum);
    // creating an empty string with spaces
    string empty = string(lineNumStr.size() + 1, ' ');
    // getting the max size of the error, so it doesn't go out of bounds 
    int maxSize = columnNum + size > line.size() ? line.size() - columnNum + 1: size;
    maxSize = maxSize <= 0 ? 1: maxSize;
    // column
    string column = columnNum != 0 ? string(columnNum-1, ' ') : "";
    return "" 
        + lineNumStr + " " + BORDER_VERTICAL + "\t"+ line + "\n"
        + empty + BORDER_VERTICAL + "\t" + column + string(maxSize, '^');
}

void Error::logError(string message, string error, int lineNum, int columnNum, int size, ErrorType type){
    columnNum--; // 0 based index
    if(type == ERROR)
        hasError = true;
    // stringifying the line and column
    string lineNumStr = to_string(lineNum);
    string columnNumStr = to_string(columnNum);
    // creating an empty string with spaces
    string empty = string(lineNumStr.size() + 1, ' ');
    // specific error type    
    string typeStr = type == WARNING ? "warning" : "error";
    // creating the error message
    string result = "";
    if(content != ""){
        result += typeStr + ": " + error + "\n"
            + empty + BORDER_TOP_LEFT + " " + fileName + ":" + lineNumStr + ":" + columnNumStr + "\n"
            + findError(lineNum, columnNum, size) + " " + message + "\n";
    }
    else
        result += typeStr + ": " + error + ": " + message + "\n";
    errors.push_back(result);
}

void Error::logError(string message, string error, Node* node, ErrorType type){
    logError(message, error, node->line, node->column, node->size, type);
}

bool Error::error(){
    for(string e: errors)
        cout << e << endl;
    return hasError;
}
