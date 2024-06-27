#include "../include/lexer.hpp"

#define IS_NUM (('0' <= content[pos] && content[pos] <= '9') || content[pos] == '_')
#define IS_ALPHA (('A' <= content[pos] && content[pos] <= 'z') || content[pos] == '_')
#define IS_ID (IS_NUM || IS_ALPHA)
#define LIMIT pos < contentSize
#define CONTAINS(vector, value) (find(vector.begin(), vector.end(), value) != vector.end())
#define KEYWORD(value) (keywordMap.find(value) != keywordMap.end())
#define SYMBOL(value) (symbolMap.find(value) != symbolMap.end())

Lexer::Lexer(Node* root, string source){
    this->root = root;
    this->source = source;
}

void Lexer::lex(){
    content = read();
    contentSize = content.size();
    currentNode = new Node();
    while(pos < contentSize){
        if(content[pos] == ' ')
            pos++;
        if(content[pos] == '\n')
            handleIndent();
        else if(IS_NUM)
            handleNumber();
        else if(IS_ALPHA)
            handleIdentifier();
        else if(content[pos] == '\"')
            handleString();
        else if(content[pos] == '\'')
            pos++; 
        else if(CONTAINS(symbols, content[pos]))
            handleSymbol();
        
    }
    for(Node* node : nodes)
        node->print();
}

void Lexer::addNode(){
    nodes.push_back(currentNode);
    currentNode = new Node();
}

void Lexer::handleIndent(){
    currentLine++;
    int count = 0;
    while(LIMIT && content[++pos] == ' ')
        count++;
    currentNode->valueInt = count;
    currentNode->type = NodeType::INDENT;
    addNode();
}

void Lexer::handleNumber(){
    currentNode->type = NodeType::NUMBER;
    while(LIMIT && (IS_NUM || content[pos] == '.')){
        if(content[pos] == '_') 
            pos++;
        else if(content[pos] == '.') 
            currentNode->type = NodeType::FLOAT;
        else 
            currentNode->valueStr += content[pos++];
    }
    addNode();
}

void Lexer::handleIdentifier(){
    currentNode->type = NodeType::IDENTIFIER;
    while(LIMIT && IS_ID) 
        currentNode->valueStr += content[pos++];
    if(KEYWORD(currentNode->valueStr)) 
        currentNode->type = keywordMap[currentNode->valueStr];
    addNode();
}

void Lexer::handleString(){
    bool foundEnd = false;
    while(LIMIT && !foundEnd){
        pos++;
        if(content[pos] == '\"' && (pos == 0 || content[pos - 1] != '\\'))
            foundEnd = true;
        else
            currentNode->valueStr += content[pos];  
    }
    addNode();
}

void Lexer::handleSymbol(){
    while(LIMIT && CONTAINS(symbols, content[pos]) && currentNode->valueStr.size() < symbolLength){
        currentNode->valueStr += content[pos];
        pos++;
    }
    while(currentNode->valueStr.size() > 0 && !SYMBOL(currentNode->valueStr)){
        currentNode->valueStr.pop_back();
        pos--;
    }
    currentNode->type = symbolMap[currentNode->valueStr];
    addNode();
}

string Lexer::read(){
    string content = "";
    ifstream file(source);
    if(!file.is_open()) 
        return nullptr;
    string line;
    while(getline(file, line))
        content += line + "\n";
    file.close();
    return content;
}
