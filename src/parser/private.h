#ifndef PARSER_PRIVATE_H
#define PARSER_PRIVATE_H

#include "include.h"

typedef struct{
    Compiler* compiler;
    Vector* nodes, *cmds, *layers;
    Node* current_node, *cmd;
    Node* hold_cmd, *hold_op;
    int spacing, index, layered_up;
    int starter_type, negative;
}Parser;

typedef void(*function_parser)(Parser* parser);

// tools
void parser_add_node_to_cmd(Parser* parser);
void parser_add_cmd_to_cmds(Parser* parser);
void parser_replace_node(Node* a,Node* b);
Node* parser_peek(Parser* parser);
void parser_loop(Parser* parser,Node* node,int type, int start, int end, int everything);
void parser_free_current_node(Parser* parser);
void parser_reset_cmd(Parser* parser);
// special functions
void parser_special_argument(Parser* parser);
void parser_special_array(Parser* parser);
void parser_special_block(Parser* parser);
void parser_keyword(Parser* parser);
void parser_keyword_with_value(Parser* parser); 
// function functions
void parser_function_argument(Parser* parser);
// block functions
void parser_block_end(Parser* parser);
// empty functions
void parser_empty_identifier(Parser* parser);
void parser_end(Parser* parser);
// declaration functions
void parser_declaration(Parser* parser);
void parser_declaration_end(Parser* parser);
void parser_declaration_value(Parser* parser);
void parser_declaration_argument(Parser* parser);
// type functions
void parser_type_init(Parser* parser);
void parser_type_end(Parser* parser);
void parser_type_add_value(Parser* parser);
void parser_type_add_type(Parser* parser);
// error
void parser_error_skip(Parser* parser);
// expression functions
void parser_expression_init(Parser* parser);
void parser_expression_add_negative(Parser* parser);
void parser_expression_unary_minus(Parser* parser);
void parser_expression_double_negation(Parser* parser);
void parser_expression_end(Parser* parser);
void parser_expression_argument(Parser* parser);
void parser_expression_argument_end(Parser* parser);
void parser_expression_ternary_op(Parser* parser);
// special table of functions
static function_parser special_functions[END] = {
    [ARGUMENT_START] = parser_special_argument,
    [ARRAY_START] = parser_special_array,
    [BLOCK_START] = parser_special_block,

    [FUNCTION] = parser_keyword,
    [STRUCT] = parser_keyword,

    [IF] = parser_keyword_with_value,
    [ELIF] = parser_keyword_with_value,
    [ELSE] = parser_keyword,
    [LOOP] = parser_keyword_with_value,
    [RETURN] = parser_keyword_with_value,

    [CONTINUE] = parser_keyword,
    [BREAK] = parser_keyword,
    
};
// cmd table of functions
static function_parser cmd_functions[END][6][END] = {
    [EMPTY] = {
        [0] = {
            [IDENTIFIER] = parser_empty_identifier,
            [NEW_LINE] = parser_end,
            [SEMICOLON] = parser_end,
        }
    },
    [FUNCTION] = {
        [0] = {[IDENTIFIER] = parser_add_node_to_cmd,},
        [1] = {[ARGUMENT] = parser_function_argument,},
        [2] = {
            [COLON] = parser_type_init,

            [NEW_LINE] = parser_end,
            [SEMICOLON] = parser_end,
        },
        [3] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [STRUCT]= {
        [0] = {[IDENTIFIER] = parser_add_node_to_cmd,},
        [1] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [IF] = {
        [1] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [ELIF] = {
        [1] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [ELSE] = {
        [0] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [LOOP] = {
        [1] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [BREAK] = {
        [0] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [CONTINUE] = {
        [0] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [RETURN] = {
        [1] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [FUNCTION_CALL] = {
        [1] = {[NEW_LINE] = parser_end,     [SEMICOLON] = parser_end,}
    },
    [DECLARATION] = {
        [1] = {
            [ARGUMENT] = parser_declaration_argument,
            [COLON] = parser_type_init,
            [EQUAL] = parser_declaration_value,
        },
        [2] = {
            [EQUAL] = parser_declaration_value,

            [NEW_LINE] = parser_end,
            [SEMICOLON] = parser_end,
            [COMMA] = parser_declaration_end,
            [ARGUMENT_END] = parser_declaration_end,
        },
        [3] = {
            [NEW_LINE] = parser_end,
            [SEMICOLON] = parser_end,
            [COMMA] = parser_declaration_end,
            [ARGUMENT_END] = parser_declaration_end,
        },
    },
    [TYPE] = {
        [0] = {
            [I1] = parser_type_add_type,
            [I8] = parser_type_add_type,
            [I16] = parser_type_add_type,
            [I32] = parser_type_add_type,
            [I64] = parser_type_add_type,
            [I128] = parser_type_add_type,

            [F16] = parser_type_add_type,
            [F32] = parser_type_add_type,
            [F64] = parser_type_add_type,
            [F128] = parser_type_add_type,
            [ARRAY] = parser_type_add_type,
            [IDENTIFIER] = parser_type_add_type,

        },
        [1] = {
            [EXCLAMATION] = parser_type_add_value,
            [QUESTION] = parser_type_add_value,
            [MUL] = parser_type_add_value,

            [COMMA]  = parser_type_end,
            [ARGUMENT_END] = parser_type_end,
            [NEW_LINE] = parser_type_end,
            [EQUAL] = parser_type_end,
            [SEMICOLON] = parser_type_end,

        },
    },
    [EXPRESSION] = {
        [0] = {
            [IDENTIFIER] = parser_add_node_to_cmd,
            [INTEGER] = parser_add_node_to_cmd,
            [FLOAT] = parser_add_node_to_cmd,

            [SUB] = parser_expression_add_negative,
        },
        [1] = {
            [ADD] = parser_expression_unary_minus,
            [SUB] = parser_expression_double_negation,
            [MUL] = parser_expression_init,
            [DIV] = parser_expression_init,
            [EQUAL_EQUAL] = parser_expression_init,
            [BANG_EQUAL] = parser_expression_init,
            [GREATER] = parser_expression_init,
            [LESS] = parser_expression_init,
            [GREATER_EQUAL] = parser_expression_init,
            [LESS_EQUAL] = parser_expression_init,
            [OR] = parser_expression_init,
            [AND] = parser_expression_init,

            [IDENTIFIER] = parser_add_node_to_cmd,
            [INTEGER] = parser_add_node_to_cmd,
            [FLOAT] = parser_add_node_to_cmd,
            [ARGUMENT] = parser_expression_argument,

            [NEW_LINE] = parser_expression_end,
            [SEMICOLON] = parser_expression_end,

            [ARGUMENT_END] = parser_expression_argument_end,
            [COMMA] = parser_expression_argument_end,

        },
        [2] = {
            [ADD] = parser_expression_unary_minus,
            [SUB] = parser_expression_double_negation,
            [MUL] = parser_expression_init,
            [DIV] = parser_expression_init,
            [EQUAL_EQUAL] = parser_expression_init,
            [BANG_EQUAL] = parser_expression_init,
            [GREATER] = parser_expression_init,
            [LESS] = parser_expression_init,
            [GREATER_EQUAL] = parser_expression_init,
            [LESS_EQUAL] = parser_expression_init,
            [OR] = parser_expression_init,
            [AND] = parser_expression_init,

            [NEW_LINE] = parser_expression_end,
            [SEMICOLON] = parser_expression_end,

            [ARGUMENT_END] = parser_expression_argument_end,
            [COMMA] = parser_expression_argument_end,

            [QUESTION] = parser_expression_ternary_op,
        },
    },
};


#endif
