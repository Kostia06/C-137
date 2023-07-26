#include "../private.h"

// helps to indeditfy which command can be left alone
static int loners[END] = {
    [FUNCTION] = 1,
    [STRUCT] = 1,
};
static int parents[END] = {
    [FUNCTION] = 1,
    [STRUCT] = 1,
    [IF] = 1,
    [ELIF] = 1,
    [ELSE] = 1,
    [LOOP] = 1,
};
// helps to indeditfy which command can have a block and which one
// if 1 then it cannot have it
static int block_holders[END][END] = {
    [FUNCTION] = {
        [STRUCT] = 1,
        [FUNCTION] = 1,
    },
    [STRUCT] = {
        [STRUCT] = 1,
        [FUNCTION] = 1,
        [FUNCTION_CALL] = 1,

        [IF] = 1,
        [ELSE] = 1,
        [LOOP] = 1,

        [BREAK] = 1,
        [CONTINUE] = 1,
        [RETURN] = 1,
    },
    [IF] = {
        [STRUCT] = 1,
        [FUNCTION] = 1,
    },
    [ELIF] = {
        [STRUCT] = 1,
        [FUNCTION] = 1,
    },
    [ELSE] = {
        [STRUCT] = 1,
        [FUNCTION] = 1,
    },
    [LOOP] = {
        [STRUCT] = 1,
        [FUNCTION] = 1,
    },
};
// if the command in the higher level then remove the command 
static int put_int_block(Parser* parser){
    for(int i=parser->layers->size-1;i>=0;i--){
        Node* block = vector_get(parser->layers,i);
        int current_block_space = block->value.integer;
        if(current_block_space < parser->cmd->value.integer){
            Node* children = vector_get(block->children,block->children->size-1);
            if(block_holders[block->type][parser->cmd->type]){
                char* message = SYNC((char*[]){PRINT_TYPE(parser->cmd->type)," cannot be in ",PRINT_TYPE(block->type),NULL});
                error_single_init(
                    parser->compiler->error,
                    SYNTAX_ERROR,
                    parser->cmd->index,
                    parser->cmd->index+parser->cmd->size,
                    message
                );
            }
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
    parser->cmd->size = node->index - parser->cmd->index;
    if(parser->layered_up){parser->layered_up = 0;}
    if(node->type == NEW_LINE){parser->spacing = node->value.integer;}
    else if(node->type == SEMICOLON){parser->layered_up = 1;}
    // clean up
    FREE_NODE(parser->compiler->memory,node);
    if(parser->cmd->type == EMPTY){return;}
    parser->cmd->value.integer = spacing;
    
    int is_loner = loners[parser->cmd->type];
    int is_parent = parents[parser->cmd->type];
    int is_in_block = put_int_block(parser);
    if(!is_loner && !is_in_block){
        char* message = SYNC((char*[]){PRINT_TYPE(parser->cmd->type)," must be in a block",NULL});
        error_single_init(
            parser->compiler->error,
            SYNTAX_ERROR,
            parser->cmd->index,
            parser->cmd->index+parser->cmd->size,
            message
        );
        parser_error_skip(parser);
        return;
    }
    if(is_parent){
        Node* block = mem_init(parser->compiler->memory,sizeof(Node));
        block->type = BLOCK;
        block->children = vector_init(parser->compiler->memory);
        vector_add(parser->cmd->children,block);
        vector_add(parser->layers,parser->cmd);
    }
    if(is_loner){parser_add_cmd_to_cmds(parser);}
    else{parser_reset_cmd(parser);}
}
