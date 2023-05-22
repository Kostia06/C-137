#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "../error/include.h"

#define MAX_HASH_SIZE 100000

#define     RESET "\033[0m"
#define     BLACK "\033[1m\033[30m"
#define     RED "\033[1m\033[31m"
#define     GREEN "\033[1m\033[32m"
#define     YELLOW "\033[1m\033[33m"
#define     BLUE "\033[1m\033[34m"
#define     MAGENTA "\033[1m\033[35m"
#define     CYAN "\033[1m\033[36m"
#define     WHITE "\033[1m\033[37m"

typedef enum{
    T_EMPTY,T_INTEGER,T_FLOAT,T_S_STRING,T_D_STRING,T_B_STRING,
    T_SIGN,T_COMMENT,
    T_IDENTIFIER,T_NEW_LINE,
    T_STRING,
    
    T_FN,T_STRUCT,T_EQUAL,T_SEMICOLON,
    T_IF,T_ELIF,    
    T_LOOP,
    T_BREAK,T_CONTINUE,
    T_RETURN, 

    T_MUTABLE,

    T_ADD,T_SUB, 
    T_MUL,T_DIV, 
    T_MOD,
    T_EQUAL_EQUAL,T_BANG_EQUAL,  
    T_GT,T_LT, 
    T_GT_EQUAL,T_LT_EQUAL,
    T_OR,T_AND, 

    T_POINTER, 
    
    T_ARGUMENT_START,T_ARGUMENT_END,T_ARGUMENT, 
    T_ARRAY_START,T_ARRAY_END,T_ARRAY, 
    
    T_COMMA, 
    
    T_I1,T_I8,T_I16,T_I32,T_I64,T_I128,
    T_F16,T_F32,T_F64,T_F128,

    T_END,
} Types;
enum{
    P_EMPTY,
    P_IDENTIFIER,
    P_NEW_LINE,

    P_COMMA,P_END_OF_ARGUMENT,

    P_PARAMETERS,
    P_TUPLE_WITH_TYPES,
    P_TUPLE_WITH_VALUES,
    P_TUPLE_WITH_IDENTIFIERS,
   
    P_TYPE_AND_IDENTIFIER,

    P_ARRAY,P_POINTER,

    P_INTEGER,P_FLOAT,P_STRING,

    P_FN,P_STRUCT, P_EQUAL,P_SEMICOLON,
    P_IF,P_ELIF,
    P_LOOP,
    P_BREAK,P_CONTINUE,
    P_RETURN,

    P_EXPRESSION, P_VARIABLE,

    P_ADD,P_SUB, 
    P_MUL,P_DIV, 
    P_MOD,
    P_EQUAL_EQUAL,P_BANG_EQUAL,  
    P_GT,P_LT, 
    P_GT_EQUAL,P_LT_EQUAL,
    P_OR,P_AND,

    P_TYPE,
    P_MUTABLE,
    P_I1,P_I8,P_I16,P_I32,P_I64,P_I128,
    P_F16,P_F32,P_F64,P_F128,

    P_END,
}ParserType;
typedef struct TokenStruct Token;
typedef struct ASTStruct AST;
typedef struct TokenStruct Token;
typedef struct{
    void** data;
    size_t size;
    size_t capacity;
}Vector;
typedef struct{
    char* file;
    int macro;
} CompilerOptions;
typedef struct TokenStruct{
    union{
        float integer;
        char* string;
    }value;
    int column, size;
    int type,line;
}Token;
typedef struct ASTStruct{
    union{
        float integer;
        char* string;
    }value;
    int line,column,size;
    int type,actual_type;
    Vector* children;
}AST;

char** ALL_SCOPES(char* scope,size_t* return_size);
char* READ_FILE(char* file_name);
char* STRINGIFY(float value);
char** SPLIT(char* string,char* split,int* return_size);

void PRINT_TOKEN(Token* token);
void PRINT_AST(AST* ast,int level);
char* TOKEN_TYPE(int type);
char* PARSER_TYPE(int type);

#endif
