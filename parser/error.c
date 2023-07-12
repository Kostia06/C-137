#include "private.h"

// cycle through the tokens until the end of the command
void parser_error_skip(Parser* parser){
    parser->cmd = mem_init(parser->memory,sizeof(AST));
    while(parser->ast->type != NEW_LINE && parser->ast->type != SEMICOLON && parser->index < parser->tokens->size){
        parser->token = vector_get(parser->tokens,parser->index++);
        function_parser f_token = parser_token_functions[parser->token->type];
        f_token(parser);
    }
}
// handle error
void parser_error(Parser* parser){
    printf("error\n"); 
}
