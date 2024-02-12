#include "include.hpp"


Parser::Parser(Compiler* compiler): compiler(compiler){
    this->compiler = compiler; 
    
    std::vector<std::vector<Node*> > cmds;
    std::vector<Node*> current_cmd;
    size_t index = 0;
    while(index < this->compiler->nodes.size()){
        Node* node = this->compiler->nodes[index];
        current_cmd.push_back(node);
        if(node->type == Type::NEW_LINE){
            cmds.push_back(current_cmd);
            current_cmd.clear();
        }
        index++;
    }
    
    for(int i = 0; i < cmds.size(); i++){
        std::vector<Node*> cmd = cmds[i];
        for(int j = 0; j < cmd.size(); j++){
            Node* node = cmd[j];
            std::cout << node->type << " ";
        }
        std::cout << std::endl;
    }

}
