#include "../private.h"

// add the current node to the command
void parser_add_node_to_cmd(Parser* parser){
    vector_add(parser->cmd->children,parser->current_node);
}
// add the current command to the commands
void parser_add_cmd_to_cmds(Parser* parser){
    vector_add(parser->cmds,parser->cmd);
    parser->cmd = mem_init(parser->memory,sizeof(Node));
    parser->cmd->type = parser->starter_type;
    parser->cmd->children = vector_init();
}
// peak at the next node
Node* parser_peek(Parser* parser){
    return vector_get(parser->nodes,parser->index);
}
// Replace Nodes
void parser_replace_node(Node* a,Node* b){
    Node temp;
    memcpy(&temp,a,sizeof(Node));
    memcpy(a,b,sizeof(Node));
    memcpy(b,&temp,sizeof(Node));
}
