#include "../private.h"

// start building a new type
void parser_type_init(Parser* parser){
    parser->hold_cmd = parser->cmd;
    parser->cmd = mem_init(parser->memory,sizeof(Node));
    parser->cmd->index = parser->current_node->index;
    parser->cmd->type = TYPE;
    parser->cmd->children = vector_init();
    // clean up
    FREE_NODE(parser->memory,parser->current_node);
}
// the end of the new type
void parser_type_end(Parser* parser){
    parser->current_node = parser->cmd;
    parser->cmd = parser->hold_cmd;
    parser->hold_cmd = NULL;
    parser_add_node_to_cmd(parser);
    // pretty much forces to other states to handle the end
    parser->index--;
}
// specify the type
void parser_type_add_type(Parser* parser){
    Node* node = parser->current_node;
    node->children = vector_init();
    parser_add_node_to_cmd(parser);
}
// add characterisitcs to the type
void parser_type_add_value(Parser* parser){
    Node* last = vector_get(parser->cmd->children,parser->cmd->children->size-1);
    vector_add(last->children,parser->current_node);
}
