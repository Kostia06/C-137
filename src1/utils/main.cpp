#include "include.hpp"


bool is_file(std::string file){
    std::ifstream f(file.c_str());
    return f.good();    
}

std::string read_file(std::string file){
    if(!is_file(file)){
        std::cout << "File does not exist" << std::endl;
        exit(1);
    }
    std::ifstream f(file.c_str());
    std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    return str;
}

std::vector<std::string> split_by_space(std::string str){
    std::vector<std::string> result;
    std::string word = "";
    for(char c: str){
        if(c == ' '){
            result.push_back(word);
            word = "";
        }
        else{ word += c; }
    }
    result.push_back(word);
    return result;
}
