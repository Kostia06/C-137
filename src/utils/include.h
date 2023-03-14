#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "../file/include.h"

#define CONFIG_FILE "config.lua"

#define MAX_HASH_SIZE 10000

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
    IDENTIFIER,BLOCK,
    ARRAY,ARGUMENT,
    FUNCTION,PIPE_FUNCTION,

    KEYWORD_START,
        FN,ENUM,VAR,USE,STRUCT,
        CONST,STATIC,
        IF,ELIF,
        WHILE,FOR,   
        RETURN,BREAK,CONTINUE,     
    KEYWORD_END,

    TYPE_START,
        INT,DOUBLE,FLOAT,
        VOID,CHAR,
    TYPE_END,

    MACRO_REPLACE,
    MACRO_FUNCTION,
    MACRO_DEFINE,


    LPAREN, RPAREN, // ()
    LBRACE, RBRACE, // {}
    LBRACKET, RBRACKET, // []
    COMMA, // ,
    BANG, //!
    LT, GT, // < >
    LT_EQUAL, GT_EQUAL, // <= >=
    BANG_EQUAL, EQUAL_EQUAL, // != ==
    PIPE_PIPE, AMPERSAND_AMPERSAND, // || &&
    SKIP,
    NEW_LINE,SEMICOLON, // \n ;
    PIPE, AMPERSAND, // | &

    ACTION_START,
        PLUS, MINUS, // + -
        STAR, BACKSLASH, // * /
        CARET, PERCENT, // ^ %
    ACTION_END,

    EQUAL, // =

    END,
} Types;
typedef struct NodeBlockStruct NodeBlock;
typedef struct NodeStruct Node;
typedef struct{
    char* input_file;
    char* output_file;
    char* c_flags;
    char* compiler_mode;
    int configured;
}Config;
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
typedef struct{
    void* value;
    int type,column,line;
}Token;
typedef struct{
    Token** tokens,*token;
    HashTable* table;
    char current_char;
    char *text,*current_file;
    int last_type,negative;
    size_t token_size,index,text_size,spacing;
    size_t line,column;
}Lexer;
typedef struct NodeBlockStruct{
    Node** nodes;
    size_t node_size;
    NodeBlock** blocks;
    size_t block_size;
    int type,spacing;
    int line, index;
    char* scope;
}NodeBlock;
typedef struct NodeStruct{
    void* value;
    int type,line,column;
    Node** nodes;
    size_t node_size;
}Node;

void ERROR(int condition,int line,char **message,const char* fun,char* scope);
char* SINGLE_STRING(char**array);
char* STRINGIFY(int value);
int INTEGERIFY(char* value);
char* STRINGIFY_CHAR(char value);
char* LOWER(char* string);
char** SPLIT(char* string,char* split,int* return_size);
void PRINT_TOKEN(Token* token);
void PRINT_BLOCK(NodeBlock* block,int level);
void PRINT_NODE(Node* node,int level);
char* VALUE(void* value,int type);
char* TYPE(int type);

#endif