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
// Loop through the nodes until you find end 
Node* parser_loop(Parser* parser,int type, int start, int end){
    Node* node = mem_init(parser->memory,sizeof(Node));
    node->type = type;
    node->index = parser->current_node->index;
    node->children = vector_init();
    FREE_NODE(parser->memory,parser->current_node);
    Node* child;
    // gets every node inside the argument including the end of the argument, not the start of it
    int count = 1;
    for(;parser->index<(int)parser->nodes->size;parser->index++){
        child = vector_get(parser->nodes,parser->index);
        if(child->type == NEW_LINE){
            FREE_NODE(parser->memory,child); 
            continue;
        }
        vector_add(node->children,child);
        if(child->type == start){count++;}
        else if(child->type == end){count--;}
        if(count == 0){break;}
    }
    node->size = child->index - node->index + child->size;
    // handles the closing bracket
    if(count != 0){
        char* message = SYNC((char*[]){"Expected a closing bracket, got",NULL});
        error_single_init(parser->error,SYNTAX_ERROR,node->index,node->index+node->size,message);
        parser_error_skip(parser);
        return NULL;
    }
    // checks if the argument is empty
    if(node->children->size == 1){
        child = vector_pop(node->children);
        // clean up
        FREE_NODE(parser->memory,child);
    }
    return node;
}

