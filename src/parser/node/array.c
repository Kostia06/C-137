#include "../private.h"

// Capture the array
void parser_special_array(Parser* parser){
    parser_loop(parser,parser->current_node,ARGUMENT,ARGUMENT_START,ARRAY_END,0); 
}

