#include "../private.h"

// Add current ast to the current command
void parser_add_ast_to_cmd(Parser* parser){
    vector_add(parser->cmd->children,parser->ast);
    parser->ast = mem_init(parser->memory, sizeof(AST));
}
// Add current command to the array of the commands
void parser_add_cmd_to_cmds(Parser* parser){
    if(parser->cmd->children != NULL && parser->cmd->type == EMPTY){
        char* message = SYNC((char*[]){"Command does not have a command type",NULL});
        error_init(parser->error,ERROR,parser->cmd->line,parser->cmd->column,parser->cmd->size,message);
        parser_error_skip(parser);
        return;
    }
    vector_add(parser->cmds,parser->cmd);
    parser->cmd = mem_init(parser->memory, sizeof(AST));
    parser->cmd->type = parser->starter_type;
    parser->cmd->actual_type = parser->starter_type;
    if(parser->starter_type == EXPRESSION){
        parser->cmd->children = vector_init();
    }
}
// Replace ASTS
void parser_replace_ast(AST* a,AST* b){
    AST temp;
    memcpy(&temp,a,sizeof(AST));
    memcpy(a,b,sizeof(AST));
    memcpy(b,&temp,sizeof(AST));    
}
// New ast
AST* parser_new_ast(Parser* parser,int type,int children){
    AST* ast = mem_init(parser->memory, sizeof(AST));
    ast->type = type;
    ast->actual_type = type;          
    if(children){ast->children = vector_init();}
    return ast;
}
// free
void parser_free_ast(MemoryGroup* memory,AST* ast){
    mem_free(memory,ast->value.string);
    if(ast->children != NULL){
        for(int i = 0; i <(int)ast->children->size; i++){
            parser_free_ast(memory,vector_get(ast->children,i));
        }
    }
    if(ast->children != NULL){free(ast->children);}
    mem_free(memory,ast);
}
void parser_free_token(Parser* parser){
    switch(parser->token->type){
        case STRING: case INTEGER: case FLOAT: case IDENTIFIER:
            mem_free(parser->token_memory,parser->token->value.string);
            break;
    }
    mem_free(parser->token_memory,parser->token);
}
