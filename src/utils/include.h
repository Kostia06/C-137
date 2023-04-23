#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "../file/include.h"

#define MAX_HASH_SIZE 100000
#define MAX_STACK_SIZE 10000

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
<<<<<<< HEAD
    T_EMPTY,T_INTEGER,T_FLOAT,T_S_STRING,T_D_STRING,T_B_STRING,
    T_SIGN,T_COMMENT,
    T_IDENTIFIER,T_NEW_LINE,
    T_SEMICOLON,T_STRING,
    
    T_IF,T_ELIF,    
    T_LOOP,
    T_BREAK,T_CONTINUE,
    T_RETURN, 

    T_ADD,T_SUB, 
    T_MUL,T_DIV, 
    T_EQUAL_EQUAL,T_BANG_EQUAL,  
    T_GT,T_LT, 
    T_OR,T_AND, 
=======
    EMPTY,INTEGER,STRING,SYMBOL,
    IDENTIFIER,
    ARRAY,ARGUMENT,
    
    ACTION_START,
        PUBLIC,MODULE,
        FUNCTION,VARIABLE,  
        IF,ELIF,    
        PUSH,PUT,
        LOOP,
        BREAK,CONTINUE,
        PLUS,MINUS, // + -
        MULTIPLY,DIVIDE, // * /
        EQUAL_EQUAL, BANG_EQUAL, // == != 
        GT,LT, // > <
        OR,AND, // || &&
        MACRO_REPLACE,
    ACTION_END,
    POP,

    ARGUMENT_START,ARGUMENT_END,
    FUNCTION_START,FUNCTION_END,
    ARRAY_START,ARRAY_END,
    COMMA, // ,
    SKIP,
    NEW_LINE,SEMICOLON, // \n ;
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617

    T_EQUAL, 
    
    T_ARGUMENT_START,T_ARGUMENT_END,T_ARGUMENT, 
    T_ARRAY_START,T_ARRAY_END,T_ARRAY, 
    
    T_COMMA, 
    
    T_TYPE_START,
        T_I8,T_I16,T_I32,T_I64,
    T_TYPE_END,

    T_END,
} Types;
<<<<<<< HEAD
enum{
    P_EMPTY,
    P_IDENTIFIER,
    P_INTEGER,
    P_FLOAT,

    P_ADD,
    P_SUB,
    P_MUL,
    P_DIV,

    P_TYPE,
    P_POINTER,
    P_ARRAY,
    P_ARGUMENT,

    P_VARIABLE_DECLARATION,
    P_FUNCTION_DECLARATION,
    P_FUNCTION_CALL,
    P_IF,P_ELIF,
    P_LOOP,
    P_BREAK,P_CONTINUE,
    P_RETURN,
    
    P_END,
}ParserType;
typedef struct TokenStruct Token;
typedef struct ASTStruct AST;
typedef struct CommandStruct Command;
typedef struct CommandStruct{
    int declaration,type,line,spacing;
    size_t ast_size,command_size;
    AST** asts;
    Command** commands;
}Command;
=======
typedef struct BytecodeStruct Bytecode;
typedef struct TokenStruct Token;
typedef struct BytecodeStruct{
    int type;
    void* value;
    Bytecode** values;
    Bytecode** children;
    size_t value_size,children_size;
}Bytecode;
typedef struct{
    int elements[MAX_STACK_SIZE];
    int top;
}Stack;
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
typedef struct{
    char* file;
    int macro;
} CompilerOptions;
<<<<<<< HEAD
=======
typedef struct{
    void* value;
    int type;
}Memory;
typedef struct {
    Memory* value;
} HashEntry;
typedef struct{
    int size,ids[MAX_HASH_SIZE];
    HashEntry* entries[MAX_HASH_SIZE];
} HashScope;
typedef struct{
    char* names[MAX_HASH_SIZE];
    int size,ids[MAX_HASH_SIZE];
    HashScope* entries[MAX_HASH_SIZE];
} HashTable;
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
typedef struct TokenStruct{
    union{
        float integer;
        char* string;
        Token** children;
    }value;
    size_t size;
    int type,line;
}Token;
typedef struct{
    CompilerOptions* options;
<<<<<<< HEAD
    Token** tokens,*token;
    char current_char,*text,*current_file;
    size_t token_size,index,text_size;
=======
    Token** tokens,*token,*hold;
    int last_type;
    char current_char,*text,*current_file;
    size_t token_size,index,text_size,spacing;
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
    size_t line,column;
}Lexer;
typedef struct ASTStruct{
    union{
        float integer;
        char* string;
    }value;
    int type,actual_type,line;
    size_t size;
    char* scope;
    AST** children;
}AST;

char** ALL_SCOPES(char* scope,size_t* return_size);
void ERROR(int condition,int line,char **message,const char* fun,char* scope);
char* STRINGIFY(float value);
char** SPLIT(char* string,char* split,int* return_size);
<<<<<<< HEAD

void PRINT_COMMAND(Command* command,int level);
=======
void PRINT_BYTECODE(Bytecode* bytecode,int level);
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
void PRINT_TOKEN(Token* token,int level);
void PRINT_AST(AST* ast,int level);
char* TOKEN_TYPE(int type);
char* PARSER_TYPE(int type);

#endif
