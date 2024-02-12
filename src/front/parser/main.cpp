#include "include.hpp"
#include "../../compiler/include.hpp"


Parser::Parser(Compiler* compiler): compiler(compiler){
    this->compiler = compiler; 
    std::vector<CMD*> cmds = create_cmds();
    for(CMD* cmd: cmds){ cmd -> print(); }
}


std::vector<CMD*> Parser::create_cmds(){
   std::vector<CMD*> cmds; 
   CMD* cmd = new CMD();
   size_t index = 0;
   while(index < compiler->nodes.size()){
        Node* node = compiler->nodes[index];
        cmd->add_node(node);
        if(node -> type == Type::SEMICOLON || node -> type == Type::NEW_LINE){
            cmds.push_back(cmd);
            cmd = new CMD();
        }
        index++;
        cmds.push_back(cmd);
   }
   return cmds;
}
