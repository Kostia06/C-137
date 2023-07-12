#include "include.h"
#include "private.h"

#define DEBUG_TOKEN       0

// Where parser starts
Vector* parse(ErrorGroup* error,MemoryGroup* token_memory,MemoryGroup* memory,Vector* tokens,int type, char* scope){
    Parser* parser = mem_init(memory, sizeof(Parser));
    // init memory and error
    parser->memory = memory;
    parser->token_memory = token_memory;
    parser->error = error;
    // tokens and scope
    parser->tokens = tokens;
    parser->scope = scope;
    // Current command and AST
    parser->ast = mem_init(memory, sizeof(AST));
    parser->cmd = mem_init(memory, sizeof(AST));
    parser->cmd->type = type;
    parser->starter_type = type;
    if(type == EXPRESSION){parser->cmd->children = vector_init();}
    // Stores every command
    parser->cmds = vector_init();
    // Helps to indetify where to put the command, in which block
    parser->layers = vector_init();
    while(parser->index < parser->tokens->size){
        parser->token = vector_get(parser->tokens,parser->index++);
        #if DEBUG_TOKEN == 1
            printf("size: %lu\t",parser->cmd->children != NULL ? parser->cmd->children->size : 0);
            printf("%s\t",PRINT_TYPE(parser->cmd->type));
            PRINT_TOKEN(parser->token);
        #endif
        function_parser f_token = parser_token_functions[parser->token->type];
        f_token(parser);
        if(parser->ast->type != EMPTY){
            if(parser->cmd->children == NULL || parser_functions[parser->cmd->type] == NULL){
                char* message = SYNC((char*[]){"Bad command",NULL});
                error_init(parser->error,ERROR,parser->ast->line,parser->ast->column,parser->ast->size,message);
                parser_error_skip(parser);
                parser_error(parser);
                continue;
            }
            if(parser_functions[parser->cmd->type][parser->cmd->children->size] == NULL){
                char* message = SYNC((char*[]){PRINT_TYPE(parser->cmd->type)," doesn't belong here",NULL});
                error_init(parser->error,ERROR,parser->ast->line,parser->ast->column,parser->ast->size,message);
                parser_error_skip(parser);
                parser_error(parser);
                continue;
            }
            function_parser f_parser = parser_functions[parser->cmd->type][parser->cmd->children->size][parser->ast->type];
            if(f_parser == NULL){
                char* message = SYNC((char*[]){PRINT_TYPE(parser->cmd->type)," doesn't belong here",NULL});
                error_init(parser->error,ERROR,parser->ast->line,parser->ast->column,parser->ast->size,message);
                parser_error_skip(parser);
                parser_error(parser);
                continue;
            }
            f_parser(parser);
        }
    }
    Vector* cmds = parser->cmds;
    // clean up
    //free(parser->layers);
    parser_free_ast(memory,parser->ast);
    parser_free_ast(memory,parser->cmd);
    mem_free(memory,parser);
    return cmds;
}

