#include "include.hpp"


void Parser::parse(){
    current_cmd->indent = last_indent;
    // get the last node either semicolon or new line
    Node* last_node = current_cmd->nodes.back();
    current_cmd->nodes.pop_back();
    // if it's a empty command
    if(current_cmd->nodes.size() == 0){ goto end; }
    
     
    std::cout << "CMD: " << std::endl;
    for(Node* node: current_cmd->nodes){ node->print(1); }        



    // end
    end:
        if(last_node->type == Type::NEW_LINE){  
            last_indent = std::stoi(last_node->value);
        }
}
