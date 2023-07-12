#include "../private.h"

// This helps to determine if a command is allowed to be outside of a block
// If it is allowed then it is not allowed to be inside a block
static int loners[END] = {
    [PUB] = 1,
    [FUNCTION] = 1,
};
// Helps to understand if a command is allowed to have children
static int parents[END] = {
    [FUNCTION] = 1,
    [PUB] = 1,
    [IF] = 1,
    [ELSE] = 1,
    [LOOP] = 1,
};
// Puts the command inside the block 
// If the command is in heigher layer then remove bloc
static int put_in_block(Parser* parser){
    for(int i=parser->layers->size-1;i>=0;i--){
        AST* block = vector_get(parser->layers,i);
        int current_block_space = block->value.integer;
        if(current_block_space < parser->cmd->value.integer){
            AST* children = vector_get(block->children,block->children->size-1);
            vector_add(children->children,parser->cmd);
            return 1;
        }
        vector_pop(parser->layers);
    }
    return 0;    
}
// Handle the end of the command, new line or semicolon
void parser_cmd_end(Parser* parser){
    parser_free_token(parser); // clean the token
    parser->cmd->value.integer = parser->ast->value.integer;
    mem_free(parser->memory,parser->ast);
    if(parser->layered_up==1){
        parser->layered_up = 0;
        parser->cmd->value.integer++;
    }
    if(parser->ast->type == SEMICOLON){
        parser->layered_up = 1;
    }
    int is_loner = loners[parser->cmd->actual_type];
    int is_parent = parents[parser->cmd->actual_type];
    int is_in_block = put_in_block(parser);
    if(is_loner && is_in_block){
        char* message = SYNC((char*[]){"Command is not allowed to be in a block",NULL});
        error_init(parser->error,ERROR,parser->cmd->line,parser->cmd->column,1,message);
        parser->ast = mem_init(parser->memory, sizeof(AST));
        parser->cmd = mem_init(parser->memory, sizeof(AST));
        parser->cmd->type = parser->starter_type;
        return;
    }
    if(!is_loner && !is_in_block){
        char* message = SYNC((char*[]){"Command is not allowed to be outside of a block",NULL});
        error_init(parser->error,ERROR,parser->cmd->line,parser->cmd->column,1,message);
        parser->ast = mem_init(parser->memory, sizeof(AST));
        parser->cmd = mem_init(parser->memory, sizeof(AST));
        parser->cmd->type = parser->starter_type;
        return;
    }
    if(is_parent){
        AST* block = mem_init(parser->memory, sizeof(AST));
        block->type = EMPTY;
        block->children = vector_init();
        vector_add(parser->cmd->children,block);
        vector_add(parser->layers,parser->cmd);
    }
    if(is_loner){parser_add_cmd_to_cmds(parser);}
    if(!is_loner){parser->cmd = mem_init(parser->memory, sizeof(AST));}
    // clean up
    parser->ast = mem_init(parser->memory, sizeof(AST));
    parser->cmd->type = parser->starter_type;
}
