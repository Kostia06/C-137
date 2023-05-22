#include "../private.h"

static int spacing;
static int parser_check_block(int type){
    switch(type){
        case P_FN:return 1;
        case P_IF:return 1;
        case P_ELIF:return 1;
        case P_LOOP:return 1;
        case P_STRUCT:return 1;
    }
    return 0;
}
static int parser_put_cmd_to_block(Parser* parser){
    for(int i=parser->blocks->size-1;0<=i;i--){
        AST* ast = vector_get(parser->blocks,i);
        int current_spacing = ast->value.integer;
        if(current_spacing < parser->main_ast->value.integer){
            vector_add(ast->children,parser->main_ast);
            return 1;
        }
        else{
            vector_pop(parser->blocks);
        }
    }
    return 0;
}
void parser_build_cmd(Parser* parser){
    parser->main_ast->line = parser->token->line;
    parser->main_ast->column = 0;
    parser->main_ast->size = 1;
    parser->main_ast->value.integer = spacing;
    {
        AST* last = vector_get(parser->blocks,parser->blocks->size - 1);
        char* error[] = {"ELIF has to be after IF",NULL};
        int e_condition = parser->main_ast->type == P_ELIF && (last == NULL || last->type != P_IF);
        ERROR(e_condition,parser->ast->line,parser->ast->column,parser->ast->size,error,parser->scope);
    } 
    int block_found = parser_put_cmd_to_block(parser);
    int is_block = parser_check_block(parser->main_ast->type);
    if(parser->ast->type == P_NEW_LINE){spacing = parser->token->value.integer;}
    parser_advance(parser);
    {
        char* error[] = {"This command is not allowed here",NULL};
        int type = parser->main_ast->type;
        int e_condition = !block_found && is_block && (type != P_FN && type != P_STRUCT);
        ERROR(e_condition,parser->ast->line,parser->ast->column,parser->ast->size,error,parser->scope);
    }
    if(is_block){
        vector_add(parser->blocks,parser->main_ast);
    }
    if(is_block && !block_found){
        vector_add(parser->asts,parser->main_ast);
    }
    parser_reset_main(parser);
    parser->ast = malloc(sizeof(AST));
}

void parser_token_end(Parser* parser){
    if(parser->token->type == T_NEW_LINE && (parser->main_ast->children == NULL || parser->main_ast->children->size == 0)){
        spacing = parser->token->value.integer;        
        parser_advance(parser);
        return;
    }
    else if(parser->token->type == T_SEMICOLON && (parser->main_ast->children == NULL || parser->main_ast->children->size == 0)){
        parser_advance(parser);
        return;
    }
    parser->ast->type = parser->token->type == T_NEW_LINE ? P_NEW_LINE : P_SEMICOLON;
    parser->ast->actual_type = parser->ast->type;
    parser->ast->value.string = parser->token->value.string;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = 1;
}
