#include "../private.h"

void parser_colon(Parser* parser){
    if(parser->cmd->type == DECLARATION){
        parser->cmd->type = DECLARATION_ASSIGNMENT;
    }
    parser->hold_cmd = parser->cmd;
    parser->cmd = mem_init(parser->memory, sizeof(AST));
    parser->cmd->type = TYPE;
    parser->cmd->actual_type = TYPE;
    parser->cmd->children = vector_init();
    parser->cmd->line = parser->ast->line;
    parser->cmd->column = parser->ast->column;
    parser->cmd->size = -1;
}
