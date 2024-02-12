#pragma once

#include "../../utils/include.hpp"
#include "../../node/include.hpp"
#include "../../error/include.hpp"

// shadowing Compiler
class Compiler;
class CMD;


class CMD{
    private:
    public:
        std::vector<Node*> nodes;
        std::vector<CMD*> cmds;

        CMD();
        void add_node(Node* node);
        void add_cmd(CMD* cmd);
        void print(int level = 0); 
};

class Parser{
    private:
        Compiler* compiler;

    public:
        Parser(Compiler* compiler);
        std::vector<CMD*> create_cmds();

};
