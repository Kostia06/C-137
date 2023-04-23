#ifndef PARSER_TYPE_H
#define PARSER_TYPE_H

#include "../utils/include.h"

typedef struct{
    int index,state;
    size_t token_size, command_size;
    char* scope;
    Token* current;
    Token** tokens;
    Command** commands,*command;
}Parser;
typedef void (*parser_function)(Parser* parser); 


void parser_add_identifier(Parser* parser);
void parser_argument(Parser* parser);
void parser_add_number(Parser* parser);
void parser_add_action(Parser* parser);
void parser_create_type(Parser* parser);
void parser_type_add_child(Parser* parser);
void parser_too_many_type(Parser* parser);
void parser_give_command_type(Parser* parser);
void parser_parameters(Parser* parser);
void parser_declaration(Parser* parser);
void parser_new_line(Parser* parser);
void parser_semicolon(Parser* parser);

parser_function states[P_END][T_END] = {
    [P_EMPTY] = {
        [T_I8] = parser_create_type,
        [T_I16] = parser_create_type,
        [T_I32] = parser_create_type,
        [T_I64] = parser_create_type,
        
        [T_IDENTIFIER] = parser_add_identifier,
        [T_INTEGER] = parser_add_number,
        [T_FLOAT] = parser_add_number,

        [T_ADD] = parser_add_action,
        [T_SUB] = parser_add_action,
        [T_MUL] = parser_add_action,
        [T_DIV] = parser_add_action,
        
        [T_IF] = parser_give_command_type,
        [T_ELIF] = parser_give_command_type,
        [T_LOOP] = parser_give_command_type,
        [T_RETURN] = parser_give_command_type,
        [T_BREAK] = parser_give_command_type,
        [T_CONTINUE] = parser_give_command_type,
    
        [T_SEMICOLON] = parser_semicolon,   
        [T_NEW_LINE] = parser_new_line,
    
        [T_EQUAL] = parser_declaration,

        [T_ARGUMENT] = parser_argument,
    },
    [P_TYPE] = {
        [T_MUL] = parser_type_add_child,
        [T_ARRAY] = parser_type_add_child,
        [T_I8] = parser_too_many_type,
        [T_I16] = parser_too_many_type,
        [T_I32] = parser_too_many_type,
        [T_I64] = parser_too_many_type,
    },
};

#endif

