#include "../include/lexer.hpp"

#define DEBUG 0
#define DEBUG_TOKENS DEBUG || 0
#define DEBUG_ITERATION DEBUG || 0
#define DEBUG_TOKEN_ADD DEBUG || 0

#define IS_NUM (('0' <= content[pos] && content[pos] <= '9') || content[pos] == '_')
#define IS_ALPHA (('A' <= content[pos] && content[pos] <= 'z') || content[pos] == '_')
#define IS_ID (IS_NUM || IS_ALPHA)
#define LIMIT pos < contentSize && pos >= 0
#define CONTAINS(vector, value) (find(vector.begin(), vector.end(), value) != vector.end())
#define KEYWORD(value) (keywordMap.find(value) != keywordMap.end())
#define SYMBOL(value) (symbolMap.find(value) != symbolMap.end())
#define NEW_NODE(_type) Node* node = new Node();\
    node->line = line; \
    node->column = column; \
    node->type = _type; 

Lexer::Lexer(Node* root, string source){
    this->root = root;
    this->currentNode = root;
    this->source = source;
}

void Lexer::lex(){
    content = readFile(source);
    contentSize = content.size();
    int counter = 0;
    while(pos < contentSize){

        #if DEBUG_ITERATION
            cout << "char: <" << (content[pos] == '\n' ? "new_line" : string(1, content[pos])) << "> at " + to_string(line) + ":" + to_string(column) << endl;
        #endif

        if(content[pos] == ' ')
            handleIndent();
        else if(content[pos] == '\n')
            handleNewLine();
        else if(IS_NUM)
            handleNumber();
        else if(IS_ALPHA)
            handleIdentifier();
        else if(content[pos] == '"' || content[pos] == '\'')
            handleString();
        else if(CONTAINS(symbols, content[pos]))
            handleSymbol();
        else
            handleUknown();
    }
    #if DEBUG_TOKENS
        while(root){
            root->print();
            root = root->right;
        }
    #endif
}

void Lexer::addNode(Node* node){
    #if DEBUG_TOKEN_ADD
        cout << "adding node: ";
        node->print();
    #endif
    currentNode->right = node;
    currentNode = node;
}

void Lexer::next(){
    pos++; column++;
    if(LIMIT && content[pos] == '\n'){
        line++;
        column = 1;
    }
}

void Lexer::prev(){
    pos--; column--;
}

void Lexer::handleIndent(){
    NEW_NODE(NodeType::INDENT);
    int indent = 0;
    while(LIMIT && content[pos] == ' '){
        indent++;
        node->size++;
        next();
    }
    if(currentNode->type == NodeType::NEWLINE){
        node->valueInt = indent;
        node->size = indent;
        addNode(node);
    }
}

void Lexer::handleNewLine(){
    NEW_NODE(NodeType::NEWLINE);
    node->size = 1;
    addNode(node);
    next();
}

void Lexer::handleNumber(){
    NEW_NODE(NodeType::NUMBER);
    bool invalidFloat = false;
    while(LIMIT && IS_NUM){
        node->size++;
        if(content[pos] == '.' && node->type == NodeType::FLOAT)
            invalidFloat = true;
        else if(content[pos] == '.')
            node->type = NodeType::FLOAT;
        else if(content[pos] != '_')
            node->valueInt =  (content[pos] - '0') + node->valueInt * 10;
        next();
    }
    if(invalidFloat){
        logInFileError(source, ERROR_INVALID_FLOAT, ERROR_SYNTAX, node);
        return;
    }
    addNode(node);
}

void Lexer::handleIdentifier(){
    NEW_NODE(NodeType::IDENTIFIER);
    while(LIMIT && IS_ID){
        node->valueStr += content[pos];
        node->size++;
        next();
    }
    if(KEYWORD(node->valueStr))
        node->type = keywordMap[node->valueStr];
    addNode(node);
}

void Lexer::handleString(){
    char start = content[pos++];
    NEW_NODE(NodeType::STRING);
    while(LIMIT && content[pos] != start){
        node->size++;
        node->valueStr += content[pos];
        next();
    }
    if(!(LIMIT)){
        logInFileError(source, ERROR_UNTERMINATED, ERROR_SYNTAX, node);
        return;
    }
    next();
    addNode(node);
} 

void Lexer::handleSymbol(){
    NEW_NODE(NodeType::EMPTY);
    while(LIMIT && CONTAINS(symbols, content[pos]) && node->size < symbolLength){
        node->size++;
        node->valueStr += content[pos];
        next();
    }
    int size = node->size;
    // remove last character if it is not a symbol
    while(node->size > 0 && !SYMBOL(node->valueStr)){
        node->size--;
        node->valueStr.pop_back();
        prev();
    }
    // print error if symbol is not found
    if(node->size == 0){
        // setting this to size so that error shows the entire symbol
        node->size = size; 
        logInFileError(source, ERROR_UNKNOWN_SYMBOL,ERROR_SYNTAX, node); 
        next();
        return;
    }
    node->type = symbolMap[node->valueStr];
    addNode(node);
}

void Lexer::handleUknown(){
    logInFileError(source, ERROR_UNKNOWN_CHARACTER, ERROR_SYNTAX, line, column);
    next();
}

