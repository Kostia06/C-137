#ifndef PARSER_PRIVATE_H
#define PARSER_PRIVATE_H

#include "include.h"
#include "../utils/include.h"
#include "../vector/include.h"

typedef struct{
    AST* main_ast,*hold_ast,*ast, *hold_op;
    Token* token;
    Vector* tokens,*asts,*blocks;
    int index, init_type,skip_rule;
    char* scope;
    int keyword_offset, op_offset, type_offset;
}Parser;

typedef struct{
    int values[25];
    int size;
}Rule;

typedef struct{
    Rule rules[20];
    int size;
}RuleSet;

typedef void(*parser_function)(Parser*);

void parser_switch_ast(AST* ast1, AST* ast2);
void parser_advance(Parser* parser);
void parser_add_main_to_asts(Parser* parser);
void parser_add_ast_to_main(Parser* parser);
void parser_reset_main(Parser* parser);
void parser_add_ast_to_asts(Parser* parser);

// Token functions
void parser_token_end(Parser* parser);
void parser_token_keyword(Parser* parser);
void parser_token_keyword_with_expression(Parser* parser);
void parser_token_identifier(Parser* parser);
void parser_token_argument(Parser* parser);
void parser_token_end_argument(Parser* parser);
void parser_token_array(Parser* parser);
void parser_token_type(Parser* parser);
void parser_token_op(Parser* parser);
void parser_token_pointer(Parser* parser);
void parser_token_number(Parser* parser);
void parser_token_comma(Parser* parser);
void parser_token_equal(Parser* parser);

// Expression functions
void parser_expression_op(Parser* parser);
void parser_expression_end(Parser* parser);


void parser_empty_identifier(Parser* parser);
void parser_add_value_to_type(Parser* parser);
void parser_type_and_identifier(Parser* parser);
void parser_build_cmd(Parser* parser);

void parser_expression_op(Parser* parser);
void parser_expression_end(Parser* parser);
void parser_comma_in_expression(Parser* parser);
void parser_end_of_argument_in_expression(Parser* parser);
void parser_expression_add_expression(Parser* parser);


static RuleSet rules[P_END] = {
    [P_EMPTY] = {
        .rules = {
            {
                .values = {
                    P_TYPE,P_IDENTIFIER,P_MUL,P_ARRAY,P_TUPLE_WITH_IDENTIFIERS,
                    P_COMMA,P_END_OF_ARGUMENT,P_EQUAL,
                },
                .size = 8,
            }
        },
        .size = -1,
    },
    [P_FN] = {
        .rules = {
            {.values={P_IDENTIFIER}, .size = 1},
            {.values={P_PARAMETERS}, .size=1},
            {.values={P_POINTER}, .size=1},
            {
                .values={P_TYPE,P_TUPLE_WITH_TYPES},
                .size=2
            }, 
            {
                .values={P_NEW_LINE,P_SEMICOLON},
                .size=2
            },
        },
        .size = 5,
    },
    [P_RETURN] = {
        .rules = {
            {
                .values = {P_EXPRESSION,P_TUPLE_WITH_VALUES},
                .size = 2,
            },
            {
                .values = {P_NEW_LINE,P_SEMICOLON},
                .size = 2,
            },
        },
        .size = 2,
    },
    [P_IF] = {
        .rules = {
            {
                .values = {P_EXPRESSION,P_TUPLE_WITH_VALUES},
                .size = 2,
            },
            {
                .values = {P_NEW_LINE,P_SEMICOLON},
                .size = 2,
            },
        },
        .size = 2,
    },

    [P_ELIF] = {
        .rules = {
            {
                .values = {P_EXPRESSION,P_TUPLE_WITH_VALUES},
                .size = 2,
            },
            {
                .values = {P_NEW_LINE,P_SEMICOLON},
                .size = 2,
            },
        },
        .size = 2,
    },

    [P_LOOP] = {
        .rules = {
            {
                .values = {P_EXPRESSION,P_TUPLE_WITH_VALUES},
                .size = 2,
            },
            {
                .values = {P_NEW_LINE,P_SEMICOLON},
                .size = 2,
            },
        },
        .size = 2,
    },
    [P_EXPRESSION] = {
        .rules = {
            {
                .values = {
                    P_INTEGER,P_FLOAT,P_IDENTIFIER,P_EXPRESSION,
                    P_ADD,P_SUB,P_MUL,P_DIV,P_MOD,
                    P_EQUAL_EQUAL,P_BANG_EQUAL,
                    P_GT,P_LT,P_GT_EQUAL,P_LT_EQUAL,
                    P_OR,P_AND,
                    P_NEW_LINE,P_SEMICOLON,P_COMMA,P_END_OF_ARGUMENT,
                },
                .size = 21,
            },
        },
        .size = -1,
    },
    [P_VARIABLE] = {
        .rules = {
            [2] = {
                .values = {
                    P_NEW_LINE,P_SEMICOLON,
                },
                .size = 2,
            }
        },
        .size = 3,
    }
};
static parser_function rule_functions[P_END][P_END] = {
    [P_EMPTY] = {
        [P_IDENTIFIER] = parser_empty_identifier,
        [P_TYPE] = parser_add_ast_to_main,
        [P_MUL] = parser_add_value_to_type,
        [P_ARRAY] = parser_add_value_to_type,
        [P_COMMA] = parser_add_main_to_asts,
        [P_END_OF_ARGUMENT] = parser_add_main_to_asts,
        [P_TUPLE_WITH_IDENTIFIERS] = parser_add_ast_to_main,
    },
    [P_VARIABLE] = {
        [P_EXPRESSION] = parser_add_ast_to_main,
        [P_NEW_LINE] = parser_build_cmd,
        [P_SEMICOLON] = parser_build_cmd,
    },
    [P_FN] = {
        [P_IDENTIFIER] = parser_add_ast_to_main,
        [P_PARAMETERS] = parser_add_ast_to_main,
        [P_POINTER] = parser_add_ast_to_main,
        [P_TUPLE_WITH_TYPES] = parser_add_ast_to_main,
        [P_TYPE] = parser_add_ast_to_main,
        [P_NEW_LINE] = parser_build_cmd,
        [P_SEMICOLON] = parser_build_cmd,
    },
    [P_RETURN] = {
        [P_EXPRESSION] = parser_add_ast_to_main,
        [P_TUPLE_WITH_VALUES] = parser_add_ast_to_main,
        [P_NEW_LINE] = parser_build_cmd,
        [P_SEMICOLON] = parser_build_cmd,
    },
    [P_IF] = {
        [P_EXPRESSION] = parser_add_ast_to_main,
        [P_NEW_LINE] = parser_build_cmd,
        [P_SEMICOLON] = parser_build_cmd,
    },
    [P_ELIF] = {
        [P_EXPRESSION] = parser_add_ast_to_main,
        [P_NEW_LINE] = parser_build_cmd,
        [P_SEMICOLON] = parser_build_cmd,
    },
    [P_LOOP] = {
        [P_EXPRESSION] = parser_add_ast_to_main,
        [P_NEW_LINE] = parser_build_cmd,
        [P_SEMICOLON] = parser_build_cmd,
    },
    [P_EXPRESSION] = {
        [P_INTEGER] = parser_add_ast_to_main,
        [P_FLOAT] = parser_add_ast_to_main,
        [P_IDENTIFIER] = parser_add_ast_to_main,
        [P_EXPRESSION] = parser_expression_add_expression,
        [P_ADD] = parser_expression_op,
        [P_SUB] = parser_expression_op,
        [P_MUL] = parser_expression_op,
        [P_DIV] = parser_expression_op,
        [P_MOD] = parser_expression_op,
        [P_EQUAL_EQUAL] = parser_expression_op,
        [P_BANG_EQUAL] = parser_expression_op,
        [P_GT] = parser_expression_op,
        [P_LT] = parser_expression_op,
        [P_GT_EQUAL] = parser_expression_op,
        [P_LT_EQUAL] = parser_expression_op,
        [P_OR] = parser_expression_op,
        [P_AND] = parser_expression_op,

        [P_COMMA] = parser_expression_end,
        [P_NEW_LINE] = parser_expression_end,
        [P_SEMICOLON] = parser_expression_end,
        [P_END_OF_ARGUMENT] = parser_expression_end,
    },
}; 
static parser_function build_ast_token[T_END] = {
    //new command, the of the command
    [T_NEW_LINE] = parser_token_end,
    [T_SEMICOLON] = parser_token_end,
    //new number
    [T_INTEGER] = parser_token_number,
    [T_FLOAT] = parser_token_number,
    //new keyword, change the main_ast type to the keyword
    [T_EQUAL] = parser_token_equal,
    [T_FN] = parser_token_keyword,
    [T_STRUCT] = parser_token_keyword,
    [T_RETURN] = parser_token_keyword_with_expression,
    [T_IF] = parser_token_keyword_with_expression,
    [T_ELIF] = parser_token_keyword_with_expression,
    [T_LOOP] = parser_token_keyword_with_expression,
    //id
    [T_IDENTIFIER] = parser_token_identifier,
    //argument,array
    [T_ARGUMENT_START] = parser_token_argument,
    [T_ARGUMENT_END] = parser_token_end_argument,
    [T_ARRAY_START] = parser_token_array, 
    [T_COMMA] = parser_token_comma,
    [T_POINTER] = parser_token_pointer,
    //types
    [T_I1] = parser_token_type,
    [T_I8] = parser_token_type,
    [T_I16] = parser_token_type,
    [T_I32] = parser_token_type,
    [T_I64] = parser_token_type,
    [T_I128] = parser_token_type,
    [T_F16] = parser_token_type,
    [T_F32] = parser_token_type,
    [T_F64] = parser_token_type,
    [T_F128] = parser_token_type,
    //op
    [T_ADD] = parser_token_op, 
    [T_SUB] = parser_token_op,
    [T_MUL] = parser_token_op,
    [T_DIV] = parser_token_op,
    [T_MOD] = parser_token_op,
    [T_EQUAL_EQUAL] = parser_token_op,
    [T_BANG_EQUAL] = parser_token_op,
    [T_GT] = parser_token_op,
    [T_LT] = parser_token_op,
    [T_GT_EQUAL] = parser_token_op,
    [T_LT_EQUAL] = parser_token_op,
    [T_OR] = parser_token_op,
    [T_AND] = parser_token_op,
};

#endif
