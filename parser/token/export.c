#include "../private.h"

// just export 
void parser_token_export(Parser* parser){
    parser->cmd->actual_type = PUB;
    // clean up
    parser_free_token(parser);
}
