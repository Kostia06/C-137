#include "include.hpp"
#include "../../compiler/include.hpp"


Parser::Parser(Compiler* compiler): compiler(compiler){
    this->compiler = compiler; 
    std::vector<CMD*> cmds = create_cmds();
}


std::vector<CMD*> Parser::create_cmds(){
   std::vector<CMD*> cmds; 
   current_cmd = new CMD();
   size_t index = 0;
   while(index < compiler->nodes.size()){
        Node* node = compiler->nodes[index];
        current_cmd->add_node(node);
        if(node->type == Type::SEMICOLON || node->type == Type::NEW_LINE){
            if(current_cmd->nodes.size() > 1){ cmds.push_back(current_cmd); }
            parse();
            current_cmd = new CMD();
        }
        index++;
   }
   return cmds;
}


bool Parser::left(){
    if(index + 1 < current_cmd->nodes.size()){
        current_node = current_cmd->nodes[++index];
        return true;
    }
    return false;
}

bool Parser::right(){
    if(index - 1 >= 0){
        current_node = current_cmd->nodes[--index];
        return true;
    }
    return false;
}

Type Parser::get_type(std::string name){
    for(Keyword type: types){ if(type.name == name){ return type.type; } }
    return Type::EMPTY;
}



void CMD::add_node(Node* node){ nodes.push_back(node); }

void CMD::add_cmd(CMD* cmd){ cmds.push_back(cmd); }

void CMD::print(int tab){
    std::string tabs = "";
    for(int i = 0; i < tab; i++){ tabs += "\t"; }
    std::cout << tabs << "CMD: ";
    for(Node* node: nodes){ std::cout << "  " << node->type_str(); }
    std::cout << std::endl;
    for(CMD* cmd: cmds){ cmd->print(tab + 1); }
}



