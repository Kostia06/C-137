#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>

#define     RESET "\033[0m"
#define     RED "\033[1m\033[31m"
#define     RED_UNDERLINE "\033[4m\033[31m"
#define     MAGENTA "\033[1m\033[35m"
#define     CYAN "\033[1m\033[36m"
#define     WHITE "\033[1m\033[37m"

#define     HASH_SIZE 100000

typedef enum{
    // Types
    EMPTY, INTEGER, FLOAT, STRING, 
    ARRAY,ARGUMENT,BLOCK,
    IDENTIFIER,
    // Keywords + Command names
    FUNCTION, TYPE, EXPRESSION, SIGN, STRUCT,
    FUNCTION_PARAMETER, VARIABLE, SCOPE,
    DECLARATION, 
    IF, ELSE, LOOP,
    BREAK, CONTINUE, RETURN,

    // Types 
    I1, I8, I16,I32, I64, I128, 
    F16,F32, F64, F128,
        
    // Operators
    OP_START,
        ADD, SUB, MUL, DIV,
        EQUAL_EQUAL, BANG_EQUAL,
        GREATER, LESS, GREATER_EQUAL, LESS_EQUAL,
        OR, AND,
        
        B_AND,B_OR,
        B_XOR,
        B_LEFT,B_RIGHT,
    OP_END,
    // Signs
    
    ARGUMENT_START, ARGUMENT_END,
    ARRAY_START, ARRAY_END,
    BLOCK_START, BLOCK_END,
    
    NEW_LINE, SEMICOLON,
    QUESTION, EXCLAMATION,

    S_STRING, D_STRING, B_STRING,
    EQUAL, COMMA, COLON,
    // Comments
    SINGLE_COMMENT, SINGLE_COMMENT_START,
    MULTI_COMMENT,MULTI_COMMENT_START,
    // Error types
    SYNTAX_ERROR, FILE_ERROR, 

    

    END,
} Types;

typedef struct{
    void** data;
    size_t size;
    size_t capacity;
}Vector;

typedef struct{
    Vector* ptrs;
}MemoryGroup;

typedef struct{
    union{
        int integer;
        char* string;
    }value;
    int index, size;
    int type;
    Vector* children;
}Node;

typedef struct{
    int type, start, end;
    char* message;
} ErrorRule;

typedef struct{
    char* scope;
    Vector* rules;
} ErrorGroup;

typedef struct{
    void* value;
    int type;
} HashKey;

typedef struct{
    MemoryGroup* memory;
    HashKey* table[HASH_SIZE];
    Vector* value_keys;
} Hash;

// Tools
char* READ_FILE(ErrorGroup* error,MemoryGroup* memory,char* file_name);
char* STRINGIFY(float num);
char** SPLIT(char* string,char* split,int* return_size);
void FREE_NODE(MemoryGroup* memory,Node* node);
// Debug tools
void PRINT_NODE(Node* node,int level);
char* SYNC(char** array);
char* PRINT_TYPE(int type);
// vector
Vector* vector_init();
void vector_add(Vector* v, void* element);
void* vector_pop(Vector* v);
void* vector_pop_by_index(Vector* v,int index);
void vector_insert(Vector* v, size_t index,void* element);
void vector_clear(Vector* v);
void* vector_get(Vector* v, size_t index);
void vector_replace(Vector* v,size_t index,void* element);
void vector_remove(Vector* v,size_t index);
void vector_set_size(Vector* v,size_t size);
// memory
MemoryGroup* mem_group_init();
void* mem_init(MemoryGroup* memory,size_t size);
void* mem_copy(MemoryGroup* memory,void* ptr);
void mem_free(MemoryGroup* memory,void* ptr);
void mem_group_free(MemoryGroup* memory);
void mem_print(MemoryGroup* memory);
void vector_free(Vector* v);
// error
ErrorGroup* error_group_init(char* scope);
void error_single_init(ErrorGroup* error,int type, int start, int size, char* message);
void error_execute(ErrorGroup* error);
void error_free(ErrorGroup* error);
// hash
Hash* hash_init(MemoryGroup* memory);
int hash_id(char* key);
HashKey* hash_get(Hash* hash,int key);
void hash_add(Hash* hash, int key,int type, char* name, void* value);
int hash_find(Hash* hash,int key);
Vector* hash_get_vector(Hash* hash);

#endif
