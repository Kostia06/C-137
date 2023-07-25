#include "../private.h"

// different layers of operations
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

// give the negative trait to the next node which should be a value
void parser_expression_add_negative(Parser* parser){
    Node* next_node = parser_peek(parser);
    switch(next_node->type){
        case INTEGER: case FLOAT: case IDENTIFIER:{
            // set up children
            if(!next_node->children){next_node->children = vector_init();}
            vector_add(next_node->children,parser->current_node);
            break;
        }
        default:{
            char* message = SYNC((char*[]){"Invalid negative",NULL});
            error_single_init(parser->compiler->error,SYNTAX_ERROR,parser->index,parser->index+1,message);
            parser_error_skip(parser);
        }
    } 
}
// if the next node is minus and the current is plus then delete the current and skip to next
void parser_expression_unary_minus(Parser* parser){
    Node* next_node = parser_peek(parser);
    if(next_node->type == SUB){
        FREE_NODE(parser->compiler->memory,parser->current_node);
        return;
    }
    parser_expression_init(parser);
}
// if the next node is plus and the current is minus then delete the current and skip to next
void parser_expression_double_negation(Parser* parser){
    Node* next_node = parser_peek(parser);
    Node* prev_node = vector_get(parser->cmd->children,parser->cmd->children->size-1);
    if(next_node->type == SUB){
        FREE_NODE(parser->compiler->memory,parser->current_node);
        next_node->type = ADD;
        return;
    }
    if(prev_node && prev_node->type > OP_START && prev_node->type < OP_END){
        if(!next_node->children){next_node->children = vector_init();}
        vector_add(next_node->children,parser->current_node);
        return;
    }
    parser_expression_init(parser);
}
// this is a conditional expression
void parser_expression_ternary_op(Parser* parser){
    //parser->current_node = parser_loop(parser,ARGUMENT,QUESTION,COLON);
}

static void expression_top(Parser* parser, Node* value, Node* op){
    vector_add(op->children,value);
    vector_add(parser->hold_op->children,op);
    parser->hold_op = op;
}
static void expression_bottom(Parser* parser, Node* value, Node* op){
    vector_add(parser->hold_op->children,value);
    Node* exp = vector_pop(parser->cmd->children);
    vector_add(op->children,exp);
    vector_add(parser->cmd->children,op);
    parser->hold_op = op;
}
static void expression_middle(Parser* parser, Node* value, Node* op){
    vector_add(parser->hold_op->children,value);
    parser_replace_node(parser->hold_op,op);    
    vector_add(parser->hold_op->children,op);
}
// handle the operation in the expression state
void parser_expression_init(Parser* parser){
    Node* op = parser->current_node; 
    op->children = vector_init();
    Node* value = vector_pop(parser->cmd->children);
    // handle the first part of the expression
    if(!parser->hold_op){
        vector_add(op->children,value);
        vector_add(parser->cmd->children,op);
        parser->hold_op = op;
        return;
    }
    int hold_id = op_layers[parser->hold_op->type];
    int current_id = op_layers[op->type];
    if(hold_id > current_id){expression_top(parser,value,op);}
    else if(hold_id < current_id){expression_bottom(parser,value,op);}
    else{expression_middle(parser,value,op);}
}
// the end of the expression
void parser_expression_end(Parser* parser){
    if(!parser->hold_op){goto end;}
    Node* value = vector_pop(parser->cmd->children); 
    vector_add(parser->hold_op->children,value);
    goto end;
    end:{
        if(parser->hold_cmd){
            vector_add(parser->hold_cmd->children,parser->cmd);
            parser->cmd = parser->hold_cmd;
            parser->hold_cmd = NULL;
        }
        parser->hold_op = NULL;
        parser->index--;
    }
}
// the end in the argument
void parser_expression_argument_end(Parser* parser){
    if(!parser->hold_op){goto end;}
    Node* value = vector_pop(parser->cmd->children); 
    vector_add(parser->hold_op->children,value);
    goto end;
    end:{
        if(parser->hold_cmd){
            Node* cmd = parser->cmd;
            parser->cmd = parser->hold_cmd;
            parser->hold_cmd = NULL;
            vector_add(parser->cmd->children,cmd);
            parser->index--;
            return;
        }
        parser_add_cmd_to_cmds(parser);
        // clean up
        FREE_NODE(parser->compiler->memory,parser->current_node);
    }
}
