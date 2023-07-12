#include "../private.h"

// Create AST for new line if command is valid
void parser_token_new_line(Parser* parser){
    if(parser->cmd->type == EMPTY && parser->cmd->children != NULL && parser->cmd->children->size > 0){
        char* message = SYNC((char*[]){"Command does not have a command type",NULL});
        error_init(parser->error,ERROR,parser->token->line,parser->token->column,1,message);
        parser_error_skip(parser);
        return;
    }
    if(parser->cmd->type == EMPTY){
        parser->spacing = parser->token->value.integer;
        parser_free_token(parser); // clean up
        return;
    } 
    parser->ast->type = NEW_LINE;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
    parser->ast->value.integer = parser->spacing;
    parser->spacing = parser->token->value.integer;
}
// Create AST for semicolon
void parser_token_semicolon(Parser* parser){
    if(parser->cmd->type == EMPTY && parser->cmd->children != NULL && parser->cmd->children->size > 0){
        char* error = SYNC((char*[]){"Command does not have a command type",NULL});
        error_init(parser->error,ERROR,parser->token->line,parser->token->column,1,error);
        parser_error_skip(parser);
    }
    if(parser->cmd->type == EMPTY){
        parser_free_token(parser); // clean up
        return;
    }
    parser->ast->type = SEMICOLON;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
    parser->ast->value.integer = parser->spacing;
}
// Create AST for comma
void parser_token_comma(Parser* parser){
    parser->ast->type = COMMA;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
    // clean up
    parser_free_token(parser);
}
