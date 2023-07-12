#include "../../private.h"
#include "private.h"

// Different layers of operations
static int op_layers[END] = {
    [OR] = 4,             [AND] = 4,
    [EQUAL_EQUAL] = 4,    [BANG_EQUAL] = 4,
    [GREATER] = 4,        [LESS] = 4,
    [GREATER_EQUAL] = 4,  [LESS_EQUAL] = 4,
    [B_AND] = 4,          [B_OR] = 4,
    [B_XOR] = 4,        
    [B_LEFT] = 4,         [B_RIGHT] = 4,
    [ADD] = 3,            [SUB] = 3,
    [MUL] = 2,            [DIV] = 2,
};
static void expression_op_top(Parser* parser,AST* value,AST* current_op){
    vector_add(current_op->children,value); 
    vector_add(parser->hold_op->children,current_op);
    parser->hold_op = current_op;
}
static void expression_op_middle(Parser* parser,AST* value,AST* current_op){
    vector_add(parser->hold_op->children,value);
    op_simple(parser,parser->hold_op);   // check if there is a simple operation
    op_complex(parser,parser->hold_op); // check if there is a complex operation
    op_complex_identifier(parser,parser->hold_op); // check if there is a complex operation

    parser_replace_ast(parser->hold_op,current_op);

    vector_add(parser->hold_op->children,current_op);
}
static void expression_op_bottom(Parser* parser,AST* value,AST* current_op){
    vector_add(parser->hold_op->children,value);
    op_simple(parser,parser->hold_op); // check if there is a simple operation

    AST* exp = vector_pop(parser->cmd->children);
    op_simple(parser,exp); // check if there is a simple operation
    op_complex(parser,exp); // check if there is a complex operation

    vector_add(current_op->children,exp);
    vector_add(parser->cmd->children,current_op);
    parser->hold_op = current_op;
}
// Handle operation with value
void parser_expression_op(Parser* parser){
    // Get current operation
    AST* current_op = parser->ast;
    current_op->children = vector_init();
    parser->ast = mem_init(parser->memory, sizeof(AST));
    // Get value
    AST* value = vector_pop(parser->cmd->children);
    // Handle the first operation
    if(parser->hold_op == NULL){
        vector_add(current_op->children,value);
        vector_add(parser->cmd->children,current_op);
        parser->hold_op = current_op;
        return;
    }
    // Get the layers of the operations
    int hold_id = op_layers[parser->hold_op->type];
    int current_id = op_layers[current_op->type];
    if(hold_id > current_id){expression_op_top(parser,value,current_op);}
    else if(hold_id < current_id){expression_op_bottom(parser,value,current_op);}
    else{expression_op_middle(parser,value,current_op);}
}
// make the ast a negative number
void add_negative_to_ast(Parser* parser,AST* ast){
    if(ast->children == NULL){ast->children = vector_init();}
    AST* negative = mem_init(parser->memory, sizeof(AST));
    negative->type = SUB;
    negative->actual_type = SUB;
    vector_add(ast->children,negative);
}
// check if negative
void parser_expression_negative(Parser* parser){
    AST* last = vector_get(parser->cmd->children,parser->cmd->children->size-1);
    if(parser->cmd->children->size == 0 || (last != NULL && last->type > OP_START && last->type <OP_END)){
        parser->negative = 1;
        mem_free(parser->memory,parser->ast);
        parser->ast = mem_init(parser->memory, sizeof(AST));
        return;
    }
    parser_expression_op(parser);
}
// Add value to the operation
void parser_expression_add_ast(Parser* parser){
    AST* last = vector_get(parser->cmd->children,parser->cmd->children->size-1);
    if(last != NULL && (last->type == FLOAT || last->type == INTEGER)){
        AST* mul = parser_new_ast(parser, MUL,0);
        AST* save = parser->ast;
        parser->ast = mul;
        parser_expression_op(parser);
        parser->ast = save;
    }
    if(!parser->negative){parser_add_ast_to_cmd(parser);return;}
    parser->negative = 0;
    add_negative_to_ast(parser,parser->ast);
    parser->last_type = parser->ast->type;
    parser_add_ast_to_cmd(parser);
}
