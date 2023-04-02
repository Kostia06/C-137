#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h>

#include "../file/include.h"

#define CONFIG_FILE "config.lua"

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
    EMPTY,INTEGER,STRING,
    IDENTIFIER,
    ARRAY,ARGUMENT,
    
    ACTION_START,
        PUBLIC,MODULE,
        FUNCTION,VARIABLE,
        IF,ELIF,
        PUSH,
        LOOP,
        RETURN,BREAK,CONTINUE,
        PLUS,MINUS,    
        MULTIPLY,DIVIDE,
    ACTION_END,
    POP,
    MACRO_REPLACE,

    ARGUMENT_START,ARGUMENT_END,
    FUNCTION_START,FUNCTION_END,
    ARRAY_START,ARRAY_END,
    COMMA, // ,
    BANG, //!
    LT, GT, // < >
    LT_EQUAL, GT_EQUAL, // <= >=
    BANG_EQUAL, EQUAL_EQUAL, // != ==
    OR, AND, // || &&
    SKIP,
    NEW_LINE,SEMICOLON, // \n ;

    END,
} Types;
typedef struct TokenStruct Token;
typedef struct{
    int return_type,* arguments;
    int found;
    size_t size;
} Function;
typedef struct {
    int type;
    void* value;
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
    Token** tokens,*token;
    int last_type;
    char current_char;
    char *text,*current_file;
    size_t token_size,index,text_size,spacing;
    size_t line,column;
}Lexer;

void ERROR_LOOP(int max);
void ERROR(int condition,int line,char **message,const char* fun,char* scope);
char* SINGLE_STRING(char**array);
char* STRINGIFY(int value);
char* LOWER(char* string);
char** SPLIT(char* string,char* split,int* return_size);
void PRINT_TOKEN(Token* token,int level);
char* VALUE(void* value,int type);
char* TYPE(int type);

#endif
