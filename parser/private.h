#ifndef PARSER_PRIVATE_H
#define PARSER_PRIVATE_H

#include "../utils/include.h"

typedef struct{
    ErrorGroup* error;
    MemoryGroup* memory,*token_memory;
    AST* ast, *cmd, *hold_op, *hold_cmd;
    Token* token;
    Vector* tokens,*cmds, *layers;
    char* scope;
    int index, spacing, layered_up;
    int starter_type, negative, last_type;
}Parser;

// Token functions
typedef void(*function_parser)(Parser*);
void parser_token_keyword(Parser* parser);
void parser_token_keyword_with_value(Parser* parser);
void parser_token_new_line(Parser* parser);
void parser_token_semicolon(Parser* parser);
void parser_token_comma(Parser* parser);
void parser_token_argument(Parser* parser);
void parser_token_identifier(Parser* parser);
void parser_token_integer(Parser* parser);
void parser_token_symbol(Parser* parser);
void parser_token_export(Parser* parser);
void parser_token_equal(Parser* parser);
// Array of token functions that turns tokens into ast nodes
static function_parser parser_token_functions[END] = {
    [FUNCTION] = parser_token_keyword,
    [PUB] = parser_token_export,
    
    [IF] = parser_token_keyword_with_value,
    [ELSE] = parser_token_keyword_with_value,
    [LOOP] = parser_token_keyword_with_value,

    [BREAK] = parser_token_keyword,
    [CONTINUE] = parser_token_keyword,
    [RETURN] = parser_token_keyword_with_value,

    [NEW_LINE] = parser_token_new_line,
    [SEMICOLON] = parser_token_semicolon,
    [COLON] = parser_token_symbol,
    [QUESTION] = parser_token_symbol,   
    [COMMA] = parser_token_symbol,

    [IDENTIFIER] = parser_token_identifier,
    [INTEGER] = parser_token_integer,
    [FLOAT] = parser_token_integer,
    
    [ARGUMENT_START] = parser_token_argument,
    [ARGUMENT_END] = parser_token_symbol,

    [EQUAL] = parser_token_equal,

    [MUT] = parser_token_symbol,

    [I32] = parser_token_symbol,
    [I64] = parser_token_symbol,
    [F32] = parser_token_symbol,
    [F64] = parser_token_symbol,
    [U32] = parser_token_symbol,
    [U64] = parser_token_symbol,
    
    [ADD] = parser_token_symbol,
    [SUB] = parser_token_symbol,
    [MUL] = parser_token_symbol,
    [DIV] = parser_token_symbol,

    [EQUAL_EQUAL] = parser_token_symbol,
    [BANG_EQUAL] = parser_token_symbol,
    [GREATER] = parser_token_symbol,
    [LESS] = parser_token_symbol,
    [GREATER_EQUAL] = parser_token_symbol,
    [LESS_EQUAL] = parser_token_symbol,
    [OR] = parser_token_symbol,
    [AND] = parser_token_symbol,

    [B_AND] = parser_token_symbol,
    [B_OR] = parser_token_symbol,
    [B_XOR] = parser_token_symbol,
    [B_LEFT] = parser_token_symbol,
    [B_RIGHT] = parser_token_symbol,
};

// Parser functions
// main
void parser_add_ast_to_cmd(Parser* parser);
void parser_add_cmd_to_cmds(Parser* parser);
void parser_cmd_end(Parser* parser);
void parser_replace_ast(AST* a,AST* b);
AST* parser_new_ast(Parser* parser,int type,int children);
void parser_free_ast(MemoryGroup* memory,AST* ast);
void parser_free_token(Parser* parser);
// error
void parser_error(Parser* parser);
void parser_error_skip(Parser* parser);
// argument
void parser_argument_function(Parser* parser);
void parser_argument_function_call(Parser* parser);
void parser_argument_expression(Parser* parser);
// expression
void parser_expression_op(Parser* parser);
void parser_expression_end(Parser* parser);
void parser_expression_end_argument(Parser* parser);
void parser_expression_condition(Parser* parser);
void parser_expression_negative(Parser* parser);
void parser_expression_add_ast(Parser* parser);
// type
void parser_type_end(Parser* parser);
void parser_type_cmd_end(Parser* parser);
void parser_type_add_type(Parser* parser);
void parser_type_add_value(Parser* parser);
// colon
void parser_colon(Parser* parser);
// variable
void parser_variable_equal(Parser* parser);

static function_parser parser_functions[END][10][END] = {
    [DECLARATION] = {
        [0] = {
            [IDENTIFIER] = parser_add_ast_to_cmd,
        },
        [1] = {
            [COLON] = parser_colon,
            [ARGUMENT] = parser_argument_function,
            [EQUAL] = parser_variable_equal,
        },
    },
    [DECLARATION_ASSIGNMENT] = {
        [0] = {
            [EQUAL] = parser_variable_equal,
        },
        [1] = {
            [EQUAL] = parser_variable_equal,
        }
    },
    [TYPE] = {
        [0] = {
            [MUT] = parser_add_ast_to_cmd,
            [I32] = parser_add_ast_to_cmd,
            [I64] = parser_add_ast_to_cmd,
            [F32] = parser_add_ast_to_cmd,
            [F64] = parser_add_ast_to_cmd,
            [U32] = parser_add_ast_to_cmd,
            [U64] = parser_add_ast_to_cmd,
            [EQUAL] = parser_type_cmd_end,
        },
        [1] = {
            [I32] = parser_type_add_type,
            [I64] = parser_type_add_type,
            [F32] = parser_type_add_type,
            [F64] = parser_type_add_type,
            [U32] = parser_type_add_type,
            [U64] = parser_type_add_type,
            [MUL] = parser_type_add_value,
            
            [COMMA] = parser_type_end,
            [ARGUMENT_END] = parser_type_end,

            [NEW_LINE] = parser_type_cmd_end,
            [SEMICOLON] = parser_type_cmd_end,

            [EQUAL] = parser_type_cmd_end,
        },
    },
    [FUNCTION] = {
        [0] = {
            [IDENTIFIER] = parser_add_ast_to_cmd,
        },
        [1] = {
           [ARGUMENT] = parser_argument_function, 
        },
        [2] = {
            [COLON] = parser_colon, 
            [NEW_LINE] = parser_cmd_end,
            [SEMICOLON] = parser_cmd_end,
        },
        [3] = {
            [NEW_LINE] = parser_cmd_end,
            [SEMICOLON] = parser_cmd_end,
        },
    },
    [EXPRESSION] = {
        [0] = {
            [INTEGER] = parser_expression_add_ast,
            [FLOAT] = parser_expression_add_ast,
            [IDENTIFIER] = parser_expression_add_ast,
            [SUB] = parser_expression_negative, 
            [ARGUMENT] = parser_argument_expression,
        },
        [1] = {
            [INTEGER] = parser_expression_add_ast,
            [FLOAT] = parser_expression_add_ast,
            [IDENTIFIER] = parser_expression_add_ast,

            [OR] = parser_expression_op, 
            [AND] = parser_expression_op,
            [EQUAL_EQUAL] = parser_expression_op,
            [BANG_EQUAL] = parser_expression_op,
            [GREATER] = parser_expression_op,
            [LESS] = parser_expression_op,
            [GREATER_EQUAL] = parser_expression_op,
            [LESS_EQUAL] = parser_expression_op,

            [ADD] = parser_expression_op,
            [SUB] = parser_expression_negative,
            [MUL] = parser_expression_op,
            [DIV] = parser_expression_op,

            [B_OR] = parser_expression_op,
            [B_AND] = parser_expression_op,
            [B_XOR] = parser_expression_op,
            [B_LEFT] = parser_expression_op,
            [B_RIGHT] = parser_expression_op,

            [ARGUMENT] = parser_argument_expression,

            [COMMA] = parser_add_cmd_to_cmds,
            [ARGUMENT_END] = parser_add_cmd_to_cmds,
            [NEW_LINE] = parser_expression_end,
            [SEMICOLON] = parser_expression_end,
        },
        [2] = {
            [OR] = parser_expression_op, 
            [AND] = parser_expression_op,
            [EQUAL_EQUAL] = parser_expression_op,
            [BANG_EQUAL] = parser_expression_op,
            [GREATER] = parser_expression_op,
            [LESS] = parser_expression_op,
            [GREATER_EQUAL] = parser_expression_op,
            [LESS_EQUAL] = parser_expression_op,

            [ADD] = parser_expression_op,
            [SUB] = parser_expression_negative,
            [MUL] = parser_expression_op,
            [DIV] = parser_expression_op,

            [B_OR] = parser_expression_op,
            [B_AND] = parser_expression_op,
            [B_XOR] = parser_expression_op,
            [B_LEFT] = parser_expression_op,
            [B_RIGHT] = parser_expression_op,

            [NEW_LINE] = parser_expression_end,
            [SEMICOLON] = parser_expression_end,
            
            [ARGUMENT_END] = parser_expression_end_argument,
    
            [ARGUMENT] = parser_argument_function_call,
            //[QUESTION] = parser_expression_condition,
        }
    },
};


#endif
