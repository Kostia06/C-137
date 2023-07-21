#include "../private.h"

// Capture the block
void parser_special_block(Parser* parser){
    parser_loop(parser,parser->current_node,BLOCK,BLOCK_START,BLOCK_END,0);
}
