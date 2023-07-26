#include "include.h"
#include "private.h"

#define DEBUG   0

Vector* new_parser(Compiler* compiler,Vector* nodes,int starter_type){
    // set up for the parser
    Parser* parser = mem_init(compiler->memory,sizeof(Parser));
    // the current command set up
    parser->compiler = compiler;
    parser->cmd = mem_init(compiler->memory,sizeof(Node));
    parser->cmd->children = vector_init(parser->compiler->memory);
    parser->cmd->type = starter_type;
    parser->starter_type = starter_type;

    parser->cmds = vector_init(parser->compiler->memory);
    parser->layers = vector_init(parser->compiler->memory);
    parser->nodes = nodes; 
    // loop through the nodes
    while(parser->index < nodes->size){
        parser->current_node = vector_get(nodes,parser->index++);
        function_parser special_function = special_functions[parser->current_node->type];
        // handle special cases
        if(special_function){special_function(parser);}
        #if DEBUG      
            printf("%s-%d\t",PRINT_TYPE(parser->cmd->type), (int)parser->cmd->children->size);
            PRINT_NODE(parser->current_node,0);
        #endif
        // if node is not a NULL and the command is not empty
        if(parser->current_node && parser->current_node->type != EMPTY && parser->cmd){
            function_parser function = cmd_functions[parser->cmd->type][parser->cmd->children->size][parser->current_node->type]; 
            if(function){function(parser);}
            else{
                char* message = SYNC((char*[]){"Unexpected token in ",PRINT_TYPE(parser->cmd->type),NULL});
                error_single_init(
                    parser->compiler->error,
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
    FREE_NODE(parser->compiler->memory,parser->cmd);
    mem_free(parser->compiler->memory,parser->nodes);
    mem_free(parser->compiler->memory,parser->layers);
    mem_free(parser->compiler->memory,parser);
    return cmds;
}
