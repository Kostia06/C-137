#include "../private.h"

// Read the keyword and set the current parser command to the type
void parser_token_keyword(Parser* parser){
    // Check if the command is already initialized
    if(parser->cmd->type != EMPTY){
        char* message = SYNC((char*[]){PRINT_TYPE(parser->cmd->type)," is already set",NULL});
        error_init(parser->error,ERROR,parser->token->line,parser->token->column,parser->token->size,message);
        parser_error_skip(parser);
        return;
    }
    // Check if the command is already initialized
    if(parser->cmd->children != NULL){
        char* message = SYNC((char*[]){PRINT_TYPE(parser->token->type)," is in a wrong place",NULL});
        error_init(parser->error,ERROR,parser->token->line,parser->token->column,parser->token->size,message);
        parser_error_skip(parser);
        return;
    }
    if(parser->cmd->actual_type != PUB || parser->token->type != FUNCTION){
        parser->cmd->actual_type = parser->token->type;
    }
    parser->cmd->type = parser->token->type;
    parser->cmd->line = parser->token->line;
    parser->cmd->column = parser->token->column;
    parser->cmd->children = vector_init();
    // clean up
    parser_free_token(parser);
}
// Keyword with a value
void parser_token_keyword_with_value(Parser* parser){
    // Check if the command is already initialized
    if(parser->cmd->type != EMPTY){
        char* message = SYNC((char*[]){PRINT_TYPE(parser->cmd->type)," is already set",NULL});
        error_init(parser->error,ERROR,parser->token->line,parser->token->column,parser->token->size,message);
        parser_error_skip(parser);
        return;
    }
    // Check if the command is already initialized
    if(parser->cmd->children != NULL){
        char* message = SYNC((char*[]){PRINT_TYPE(parser->token->type)," is in a wrong place",NULL});
        error_init(parser->error,ERROR,parser->token->line,parser->token->column,parser->token->size,message);
        parser_error_skip(parser);
        return;
    }
    // the command
    parser->cmd->children = vector_init();
    parser->cmd->type = parser->token->type;
    parser->cmd->actual_type = parser->token->type;
    parser->cmd->line = parser->token->line;
    parser->cmd->column = parser->token->column;
    // hold the command
    parser->hold_cmd = parser->cmd;
    parser->cmd = mem_init(parser->memory, sizeof(AST));
    parser->cmd->type = EXPRESSION;
    parser->cmd->line = parser->token->line;
    parser->cmd->column = parser->token->column;
    parser->cmd->children = vector_init();
    // clean up
    parser_free_token(parser);
}




