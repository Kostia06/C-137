#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "hash/include.hpp"

#define     RESET   "\033[0m"
#define     WHITE   "\033[1m\033[37m"
#define     MAGENTA "\033[1m\033[35m"
#define     RED     "\033[1m\033[31m"
#define     GREEN   "\033[1m\033[32m" 
#define     YELLOW  "\033[1m\033[33m"
#define     BLUE    "\033[1m\033[34m"
#define     CYAN    "\033[1m\033[36m"

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define COMPILER_TYPES(X) \
    X(EMPTY) \
    /* token type */ \
    X(STRUCT) \
    X(TYPEDEF) \
    X(IF) X(ELSE) \
    X(WHILE) X(FOR) X(GOTO) \
    X(SWITCH) X(CASE) X(DEFAULT) \
    X(ENUM) \
    X(BREAK) X(CONTINUE) X(RETURN) \
    X(VOID) \
    X(I1) X(I8) X(I16) X(I32) X(I64) X(I128) \
    X(F16) X(F32) X(F64) X(F128) \
    /* sign type */ \
    X(DOUBLE_COLON) \
    X(SINGLE_COMMENT_START) X(MULTI_COMMENT_START) \
    X(EQUAL_EQUAL) X(BANG_EQUAL) \
    X(GREATER_EQUAL) X(LESS_EQUAL) \
    X(OR) X(AND) \
    X(B_LEFT) X(B_RIGHT) \
    /* sign type */ \
    X(ADD) X(SUB) X(MUL) X(DIV) \
    X(MOD) X(POW) \
    X(GREATER) X(LESS) \
    X(EQUAL) X(SEMICOLON) X(COLON) \
    X(QUESTION) X(EXCLAMATION) \
    X(ARG_START) X(ARG_END) \
    X(ARRAY_START) X(ARRAY_END) \
    X(BLOCK_START) X(BLOCK_END) \
    X(COMMA) X(DOT) \
    X(S_STRING) X(D_STRING) X(B_STRING) \
    /* address type */ \
    X(ADDRESS) \
    /* types */ \
    X(INTEGER) X(FLOAT) \
    X(IDENTIFIER) \
    X(STRING) \
    X(NEW_LINE) \
    X(END)


typedef enum{ COMPILER_TYPES(GENERATE_ENUM) } Type;
static const std::string CType[] = { COMPILER_TYPES(GENERATE_STRING) };


typedef struct{
    std::string name;
    Type type;
} Keyword;


// file utils
bool is_file(std::string file);
std::string read_file(std::string file);
std::vector<std::string> split_by_space(std::string str);

