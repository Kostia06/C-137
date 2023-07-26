#include "../private.h"

// add the current node to the command
void parser_add_node_to_cmd(Parser* parser){
    vector_add(parser->cmd->children,parser->current_node);
}
// reset the current cmd to the starter type
void parser_reset_cmd(Parser* parser){
    parser->cmd = mem_init(parser->compiler->memory,sizeof(Node));
    parser->cmd->type = parser->starter_type;
    parser->cmd->children = vector_init(parser->compiler->memory);
    parser->hold_cmd = NULL;
}
// add the current command to the commands
void parser_add_cmd_to_cmds(Parser* parser){
    vector_add(parser->cmds,parser->cmd);
    parser_reset_cmd(parser);
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
// Free current node
void parser_free_current_node(Parser* parser){
    FREE_NODE(parser->compiler->memory,parser->current_node);
}
// Loop through the nodes until you find end 
void parser_loop(Parser* parser,Node* node,int type, int start, int end, int everything){
    node->type = type;
    if(!node->children){node->children = vector_init(parser->compiler->memory);}
    Node* child;
    // gets every node inside the argument including the end of the argument, not the start of it
    int count = 1;
    while(count && parser->index < (int)parser->nodes->size){
        child = vector_pop_by_index(parser->nodes,parser->index);
        if(child->type == NEW_LINE){
            if(!everything){
                FREE_NODE(parser->compiler->memory,child); 
                continue;
            }
        }
        vector_add(node->children,child);
        if(child->type == start){count++;}
        else if(child->type == end){count--;}
        if(count == 0){break;}
    }
    node->size = child->index - node->index + child->size;
    // handles the closing bracket
    if(count != 0){
        char* message = SYNC((char*[]){"Expected a closing ",PRINT_TYPE(end),NULL});
        error_single_init(parser->compiler->error,SYNTAX_ERROR,node->index,node->index+node->size,message);
        parser_error_skip(parser);
        return;
    }
    // checks if the argument is empty
    if(node->children->size == 1){
        child = vector_pop(node->children);
        // clean up
        FREE_NODE(parser->compiler->memory,child);
    }
}

