#include "include.hpp"

#include "../../compiler/include.hpp"

static char symbols[] = {
    '+', '-', '*', '/', '%', 
    '(', ')', '{', '}', '[', ']', 
    ';', ':', ',', '.', 
    '<', '>', '=', '!', 
    '&', '|', 
    '^', '~', '?', '@', '#', '$', '\0'
};
static char alphabet[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '_', '\0'
};
static char numbers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};
static char hexadicimal[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'a', 'b', 'c', 'd', 'e', 'f', 
    'A', 'B', 'C', 'D', 'E', 'F', '\0'
};
static char escapes[] = {'\\', '0', 'n', 't', 'r', 'v', 'b', 'f', '\'', '\"', '\0'};
static char actual_escapes[] = {'\\', '\0', '\n', '\t', '\r', '\v', '\b', '\f', '\'', '\"', '\0'};


static int in(char c, char* arr){
    for(size_t i = 0; arr[i] != '\0'; i++){
        if(c == arr[i]){ return (int)i; }
    }
    return -1;
}


bool Lexer::get_next_char(){
    if(index < compiler->current_file_content.size()){ 
        c = compiler->current_file_content[++index];
        return true;
    }
    return false;
}

bool Lexer::get_prev_char(){
    if(index > 0){ 
        c = compiler->current_file_content[--index];
        return true;
    }
    return false;
}

bool Lexer::handle_spaces(){
    // if new line count the indents
    if(c == '\n'){
        size_t start = index;
        size_t count = 0;
        while(get_next_char() && c == ' '){ count++; }
        compiler->nodes.push_back(new Node(Type::NEW_LINE, std::to_string((int)count), start, index));
        return true;
    }
    // ignore spaces
    if(c != ' '){ return false; }
    while(get_next_char() && c == ' ');
    return true;
}

bool Lexer::handle_symbols(){
    if(in(c, symbols) == -1){ return false; }
    std::string result{ c };
    size_t start = index;
    size_t end = 0;
    // collect all symbols possible
    while(get_next_char() && in(c, symbols) != -1){ result += c; }
    end = index;
    std::string result_error = result;
    // find the sign
    while(result.size() > 0){
        for(Keyword keyword: compiler->signs){
            if(keyword.name == result){
                compiler->nodes.push_back(new Node(keyword.type, result, start, index));
                return true;
            }
        }
        result.pop_back();
        get_prev_char();
    } 
    // none find
    compiler->error_handler->one_line("unknown symbol \"" + result_error + "\"" ,compiler->current_file_name , start, end);  
    return false;
}

bool Lexer::handle_keywords(){
    if(in(c, alphabet) == -1){ return false; }
    std::string result{ c };
    size_t start = index;
    // collect all letters and numbers
    while(get_next_char() && ( in(c, alphabet) != -1 || in(c, numbers ) != -1)){ result += c; }
    size_t end = index;
    // find the keyword
    for(Keyword keyword: compiler->keywords){
        if(keyword.name == result){
            compiler->nodes.push_back(new Node(keyword.type, result, start, end));
            return true;
        }
    }
    // if no keyword found
    compiler->nodes.push_back(new Node(Type::IDENTIFIER, result, start, end));
    return true;
}

bool Lexer::handle_strings(){ 
    if(c != '\"' && c != '`'){ return false; }
    std::string result = "";
    char start_string = c;
    size_t start = index;
    size_t end = 0;
    // collect everything until the end of the string and handle escapes
    while(get_next_char() && c != start_string){
        if(c == '\\'){
            if(get_next_char()){
                int escape_index = in(c, escapes);
                if(escape_index != -1){ 
                    compiler->error_handler->one_line("unknown escape sequence", compiler->current_file_name, index, 1); 
                }
                else{ result += actual_escapes[escape_index]; }
            }
            else{ 
                compiler->error_handler->one_line("unknown escape sequence", compiler->current_file_name, index, 1); 
            }
        }

        else{ result += c; }
    }
    end = index;
    // handle the string not being closed
    if(c != start_string){
        compiler->error_handler->one_line("string not closed", compiler->current_file_name, start, end);
        return false;
    }
    compiler->nodes.push_back(new Node(Type::STRING, result, start, end));
    get_next_char();
    return true;
}

bool Lexer::handle_numbers(){
    if(in(c, numbers) == -1){ return false; }
    std::string result{ c };
    bool is_float = false;
    size_t start = index;
    size_t end = 0;
    // collect all numbers
    while(get_next_char() && ( in(c, numbers) != -1 || c == '.' || c == '_')){
        if(c == '.' && !is_float){ is_float = true; }
        else if(c == '.' && is_float){ 
            compiler->error_handler->one_line("multiple dots in a number", compiler->current_file_name, index, 1); 
            return false; 
        }
        else if(c == '_'){ continue; }
        else if(c != '_'){ continue; }
        result += c;
    }
    end = index;
    compiler->nodes.push_back(new Node(is_float ? Type::FLOAT : Type::INTEGER, result, start, end));
    return true;
}


bool Lexer::handle_addresses(){
    // check if the prefix is right 0x
    size_t start = index;
    if(c != '0'){ return false; }
    if(!get_next_char()){ return false; }
    if(c != 'x'){ return false; }
    std::string result = "0x";
    while(get_next_char() && (in(c, hexadicimal) != -1 || c == '_')){ 
        if(c == '_'){ continue; }
        result += c; 
    }
    compiler->nodes.push_back(new Node(Type::ADDRESS, result, start, index));
    return false;
}


