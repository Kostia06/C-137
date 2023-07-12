#include "../include.h"
#include "../private.h"

// Start of an argument
void parser_token_argument(Parser* parser){
    parser->ast->type = ARGUMENT;
    parser->ast->children = vector_init();
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column; 
    Vector* tokens = vector_init();
    int count = 1;
    // clean up the token
    parser_free_token(parser);
    for(;parser->index<parser->tokens->size;parser->index++){
        Token* token = vector_get(parser->tokens,parser->index);
        if(token->type == NEW_LINE){continue;}
        vector_add(tokens,token);
        if(token->type == ARGUMENT_START){count++;}
        else if(token->type == ARGUMENT_END){count--;}
        if(count == 0){
            parser->ast->size = (token->column-parser->ast->column);
            break;
        }
    }
    if(tokens->size == 1){
        parser->token = vector_pop(tokens);
        parser_free_token(parser);
    }
    if(count){
        char* message = SYNC((char*[]){"Missing end of the ARGUMENT",NULL});
        error_init(parser->error,ERROR,parser->ast->line,parser->ast->column,1,message);
        parser_error_skip(parser);
        return;
    }
    parser->ast->children = tokens;
    parser->index++;
}
