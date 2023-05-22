#include "../private.h"

static int action_id[P_END] = {
    [P_OR] = 4,[P_AND] = 4,
    [P_EQUAL_EQUAL] = 4,[P_BANG_EQUAL] = 4,
    [P_GT] = 4,[P_LT] = 4,
    [P_GT_EQUAL] = 4,[P_LT_EQUAL] = 4,
    [P_ADD] = 3,[P_SUB] = 3,
    [P_MUL] = 2,[P_DIV] = 2,
    [P_MOD] = 1,
};

void parser_token_op(Parser* parser){
    parser->ast->type = parser->token->type+parser->op_offset; 
    parser->ast->actual_type = parser->ast->type;
    parser->ast->line = parser->token->line;
    parser->ast->column = parser->token->column;
    parser->ast->size = parser->token->size;
}
void parser_expression_add_expression(Parser* parser){
    AST* last = vector_pop(parser->ast->children);
    parser->ast = last;
    parser_add_ast_to_main(parser);
}
void parser_expression_end(Parser* parser){
    if(parser->hold_ast == NULL){
        parser_add_main_to_asts(parser);
        parser->main_ast->type = P_EXPRESSION;
        parser->main_ast->actual_type = P_EXPRESSION;
    }
    else if(parser->hold_op == NULL){
        parser_switch_ast(parser->main_ast,parser->hold_ast);
        vector_add(parser->main_ast->children,parser->hold_ast);
    }
    else{
        AST* last_value = vector_pop(parser->main_ast->children);
        vector_add(parser->hold_op->children,last_value);
        parser_switch_ast(parser->main_ast,parser->hold_ast);
        vector_add(parser->main_ast->children,parser->hold_ast);
        parser->hold_op = NULL;
        parser->hold_ast = NULL;
    }
}
void parser_expression_op(Parser* parser){
    AST* op = parser->ast;
    op->children = vector_init();
    AST* value = vector_get(parser->main_ast->children,parser->main_ast->children->size-1);
    {
        char* error[] = {"Expression must have a value",NULL};
        int e_condition = value == NULL;
        ERROR(e_condition,op->line,op->column,op->size,error,parser->scope);
    }
    vector_pop(parser->main_ast->children);
    if(parser->hold_op != NULL){
        int hold_id = action_id[parser->hold_op->type];
        int op_id = action_id[op->type];
        if(hold_id>op_id){
            vector_add(op->children,value);
            vector_add(parser->hold_op->children,op);
            parser->hold_op = op;
        }
        else if(hold_id<op_id){
            vector_add(parser->hold_op->children,value);
            vector_add(op->children,vector_get(parser->main_ast->children,0));
            vector_replace(parser->main_ast->children,0,op);
            parser->hold_op = op;
        }
        else{
            vector_add(parser->hold_op->children,value);
            parser_switch_ast(parser->hold_op,op);
            vector_add(parser->hold_op->children,op);
        }

    }
    else{
        vector_add(op->children,value);   
        vector_add(parser->main_ast->children,op);
        parser->hold_op = op;
    }
    parser->ast = malloc(sizeof(AST));
    parser_advance(parser);
}
