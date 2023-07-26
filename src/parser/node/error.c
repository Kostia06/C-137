#include "../private.h"

// skips to the end of the current command to avoid even more unseseary errors
void parser_error_skip(Parser* parser){
    while(
        parser->current_node->type != NEW_LINE && 
        parser->current_node->type != SEMICOLON && 
        parser->current_node->type != ARGUMENT_END &&
        parser->current_node->type != COMMA
    ){
        parser->current_node = vector_get(parser->nodes,parser->index++);
        function_parser special_function = special_functions[parser->current_node->type];
        // handle special cases
        if(special_function){special_function(parser);}
    }
    // clean up
    parser->cmd = mem_init(parser->compiler->memory,sizeof(Node));
    parser->cmd->type = parser->starter_type;
    parser->cmd->children = vector_init(parser->compiler->memory);
    parser->hold_cmd = NULL;
}
