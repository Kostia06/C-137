#include "../private.h"

// Fully develope the argument
void parser_special_array(Parser* parser){
    parser->current_node = parser_loop(parser,ARGUMENT,ARGUMENT_START,ARRAY_END); 
}

