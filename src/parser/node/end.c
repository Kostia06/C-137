#include "../private.h"

// helps to indeditfy which command can be left alone
static int loners[END] = {
    [FUNCTION] = 1,
};
// helps to identify which command can have more commands in it
static int parents[END] = {
    [FUNCTION] = 1,
    [IF] = 1,
    [ELSE] = 1,
    [LOOP] = 1,
};
// if the command in the higher level then remove the command 
static int put_int_block(Parser* parser){
    for(int i=parser->layers->size-1;i>=0;i--){
        Node* block = vector_get(parser->layers,i);
        int current_block_space = block->value.integer;
        if(current_block_space < parser->cmd->value.integer){
            Node* children = vector_get(block->children,block->children->size-1);
            vector_add(children->children,parser->cmd);
            return 1;
        }
        vector_pop(parser->layers);
    }
    return 0;    
    
}

// handle the end of each command
void parser_end(Parser* parser){
    Node* node = parser->current_node;
    int spacing = parser->spacing + parser->layered_up;

    if(node->type == NEW_LINE){parser->spacing = node->value.integer;}
    else if(node->type == SEMICOLON){parser->layered_up = 1;}
    if(parser->layered_up){parser->layered_up = 0;}
    // clean up
    FREE_NODE(parser->memory,node);
    if(parser->cmd->type == EMPTY){return;}
    parser->cmd->value.integer = spacing;

    int is_loner = loners[parser->cmd->type];
    int is_parent = parents[parser->cmd->type];
    int is_in_block = put_int_block(parser);
    if(is_loner && is_in_block){
        char* message = SYNC((char*[]){"Command cannot be in a block",NULL});
        error_single_init(parser->error,SYNTAX_ERROR,parser->cmd->index,1,message);
        parser_error_skip(parser);
        goto end;
    }
    if(!is_loner && !is_in_block){
        char* message = SYNC((char*[]){"Command must be in a block",NULL});
        error_single_init(parser->error,SYNTAX_ERROR,parser->cmd->index,1,message);
        parser_error_skip(parser);
        goto end;
    }
    if(is_parent){
        Node* block = mem_init(parser->memory,sizeof(Node));
        block->type = EMPTY;
        block->children = vector_init();
        vector_add(parser->cmd->children,block);
        vector_add(parser->layers,parser->cmd);
    }
    if(is_loner){parser_add_cmd_to_cmds(parser);}
    goto end;
    // clean up
    end:{
        parser->cmd = mem_init(parser->memory,sizeof(Node)); 
        parser->cmd->children = vector_init();
        parser->cmd->type = parser->starter_type;
        parser->hold_cmd = NULL;
    }
    

}
