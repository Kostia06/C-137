#include "../include/utils.hpp"

string readFile(string fileName){
    string content = "";
    ifstream file(fileName);
    if(!file.is_open()) 
        return "NOT_FOUND";
    string line;
    while(getline(file, line))
        content += line + "\n";
    file.close();
    return content;
}

vector<string> split(string str, string delimiter){
    vector<string> tokens;
    size_t pos = 0;
    while((pos = str.find(delimiter)) != string::npos){
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    }
    tokens.push_back(str);
    return tokens;
}
