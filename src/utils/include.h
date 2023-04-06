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

    END,
} Types;
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
typedef struct{
    char* file;
    int macro;
} CompilerOptions;
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
typedef struct TokenStruct{
    void* value;
    Token** children;
    size_t size;
    int type,line;
}Token;
typedef struct{
    CompilerOptions* options;
    Token** tokens,*token,*hold;
    int last_type;
    char current_char,*text,*current_file;
    size_t token_size,index,text_size,spacing;
    size_t line,column;
}Lexer;

void ERROR_LOOP(int max);
void ERROR(int condition,int line,char **message,const char* fun,char* scope);
char* SINGLE_STRING(char**array);
char* STRINGIFY(int value);
char* LOWER(char* string);
char** SPLIT(char* string,char* split,int* return_size);
void PRINT_BYTECODE(Bytecode* bytecode,int level);
void PRINT_TOKEN(Token* token,int level);
char* VALUE(void* value,int type);
char* TYPE(int type);

#endif
