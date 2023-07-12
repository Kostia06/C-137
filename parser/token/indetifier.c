#include "../private.h"

// Start of a new identifier AST
void parser_token_identifier(Parser* parser){
    parser->ast->type = IDENTIFIER;
    parser->ast->value.string = mem_copy(parser->memory,parser->token->value.string);
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = parser->token->size;
    parser->ast->children = vector_init();
    if(parser->cmd->children == NULL && parser->cmd->type == EMPTY){
        parser->cmd->children = vector_init();
        parser->cmd->type = DECLARATION;
        parser->cmd->line = parser->token->line;
        parser->cmd->column = parser->token->column;
        parser->cmd->size = -1;
    }
    // clean up
    parser_free_token(parser);
}
