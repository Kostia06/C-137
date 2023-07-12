#include "include.h"
#include "private.h"

#define DEBUG   0

Vector* new_parser(ErrorGroup* error,MemoryGroup* memory,Vector* nodes,int starter_type,char* scope){
    // set up for the parser
    Parser* parser = mem_init(memory,sizeof(Parser));
    // the current command set up
    parser->cmd = mem_init(memory,sizeof(Node));
    parser->cmd->children = vector_init();
    parser->cmd->type = starter_type;
    parser->starter_type = starter_type;

    parser->cmds = vector_init();
    parser->layers = vector_init();
    parser->nodes = nodes;  
    parser->error = error;
    parser->memory = memory;
    parser->scope = scope;
    // loop through the nodes
    while(parser->index < nodes->size){
        parser->current_node = vector_get(nodes,parser->index++);
#if DEBUG      
        printf("%s-%d\t",PRINT_TYPE(parser->cmd->type), (int)parser->cmd->children->size);
        PRINT_NODE(parser->current_node,0);
#endif
        function_parser special_function = special_functions[parser->current_node->type];
        // handle special cases
        if(special_function){special_function(parser);}
        // if node is not a NULL and the command is not empty
        if(parser->current_node && parser->current_node->type != EMPTY && parser->cmd){
            function_parser function = cmd_functions[parser->cmd->type][parser->cmd->children->size][parser->current_node->type]; 
            if(function){function(parser);}
            else{
                printf("ERROR\n");
                char* message = SYNC((char*[]){"Unexpected token",NULL});
                error_single_init(
                    parser->error,
                    SYNTAX_ERROR,
                    parser->current_node->index,
                    parser->current_node->index+parser->current_node->size,
                    message
                );
                parser_error_skip(parser);
            } 
        }
        
    }
    Vector* cmds = parser->cmds;
    free(parser->layers);
    mem_free(memory,parser->cmd);
    mem_free(memory,parser);
    return cmds;
}
