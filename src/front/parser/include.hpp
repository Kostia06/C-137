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
        size_t indent = 0;
    
        void add_node(Node* node);
        void add_cmd(CMD* cmd);
        void print(int tab = 0);
};

class Parser{
    private:
        std::vector<Keyword> types;

        Compiler* compiler;
        CMD* current_cmd;
        Node* current_node;
        size_t index = 0;
        size_t last_indent = 0;

    public:
        Parser(Compiler* compiler);
        std::vector<CMD*> create_cmds();

        void parse();
        bool left();
        bool right();
        Type get_type(std::string name);
};
