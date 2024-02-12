#include "include.hpp"


CMD::CMD(){}
void CMD::add_node(Node* node){ nodes.push_back(node); }
void CMD::add_cmd(CMD* cmd){ cmds.push_back(cmd); }
void CMD::print(int level){
    for(int i = 0; i < level; i++){ std::cout << "\t"; }
    for(Node* node: nodes){ node->print_inlined(); }
    for(CMD* cmd: cmds){ cmd->print(level + 1); }
}
