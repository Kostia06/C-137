#include "../private.h"

// end of type like = or , 
void parser_type_end(Parser* parser){
    mem_free(parser->memory,parser->ast);
    parser->ast = mem_init(parser->memory, sizeof(AST));
    if(parser->hold_cmd->type == DECLARATION_ASSIGNMENT){
        vector_add(parser->hold_cmd->children,parser->cmd);
    }
    else{parser->ast = parser->cmd;}
    parser->cmd = parser->hold_cmd;
    parser->hold_cmd = NULL;
    parser_add_cmd_to_cmds(parser);
}
// add pointer / array
void parser_type_add_type(Parser* parser){
    AST* ast = vector_get(parser->cmd->children,0);
    if(ast->type != MUT){
        char* message = SYNC((char*[]){"Too many types for a single declaration",NULL});
        error_init(parser->error,ERROR,ast->line,ast->column,ast->size,message);
        parser_error_skip(parser);
        return;
    }
    if(ast->type != EQUAL){
        parser->ast->children = vector_init();
        vector_add(parser->ast->children,vector_pop(parser->cmd->children));
    }
    parser_add_ast_to_cmd(parser);
}
// add value to type
void parser_type_add_value(Parser* parser){
    AST* ast = vector_get(parser->cmd->children,0);
    if(ast->type == MUT){
        char* message = SYNC((char*[]){"Can't use pointer without specified type",NULL});
        error_init(parser->error,ERROR,ast->line,ast->column,ast->size,message);
        parser_error_skip(parser);
        return;
    }
    if(ast->children == NULL){ast->children = vector_init();}
    vector_add(ast->children,parser->ast);
    parser->ast = mem_init(parser->memory, sizeof(AST));
}
// end of the type like new line or ;
void parser_type_cmd_end(Parser* parser){
    if(parser->ast->type == NEW_LINE){
        parser->spacing = parser->ast->value.integer;
    }
    mem_free(parser->memory,parser->ast);
    if(parser->hold_cmd->type == DECLARATION_ASSIGNMENT){
        AST* last = vector_get(parser->hold_cmd->children,parser->hold_cmd->children->size-1);
        last->children = vector_init();
        vector_add(last->children,parser->cmd);
    }
    else{vector_add(parser->hold_cmd->children,parser->cmd);}
    parser->cmd = parser->hold_cmd;
    parser->hold_cmd = NULL;
    parser->ast = mem_init(parser->memory, sizeof(AST));
    parser->index--;
}
