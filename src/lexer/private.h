#ifndef LEXER_PRIVATE_H
#define LEXER_PRIVATE_H

#include "../utils/include.h"

typedef struct{
    Node *node;
    char current_char,*text,*scope;
    size_t node_size,index,text_size;
    Vector* nodes;
    MemoryGroup* memory;
    ErrorGroup* error;
}Lexer;
typedef void(*lexer_function)(Lexer*);
typedef struct{
    char* name;
    int type;
}Keyword;
enum{
    LT_SPACE,
    LT_UNDERSCORE,
    LT_SINGLE_QUOTE,LT_DOUBLE_QUOTE,LT_BACKTICK,
    LT_SIGN,
    LT_DOT, LT_DIGIT,
    LT_ALPHA,
    LT_NEW_LINE,
    LT_END,
}LexerType;
static int characters[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, LT_NEW_LINE, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	LT_SPACE, LT_SIGN, LT_DOUBLE_QUOTE, LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN,LT_SINGLE_QUOTE,
    LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_DOT, LT_SIGN,
	LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT,
	LT_DIGIT, LT_DIGIT, LT_SIGN, LT_SIGN,LT_SIGN ,LT_SIGN, LT_SIGN, LT_SIGN,
	LT_SIGN, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_SIGN, LT_SIGN, LT_SIGN,LT_SIGN, LT_UNDERSCORE,
	LT_BACKTICK, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_SIGN, LT_SIGN ,LT_SIGN, LT_SIGN,0,
};
#define KEYWORD_COUNT 23 
static Keyword keywords[] = {
    {"fn",FUNCTION},{"struct",STRUCT},
    {"if",IF},{"else",ELSE},{"loop",LOOP},
    {"vec",VECTOR},{"arr",ARRAY},
    {"break",BREAK},{"continue",CONTINUE},{"return",RETURN},
    {"i1",I1},{"i8",I8},{"i16",I16},{"i32",I32},{"i64",I64},{"i128",I128},
    {"f16",F16},{"f32",F32},{"f64",F64},{"f128",F128},
};

// IMPORTANT: the order of the signs should be from the longest to the shortest
#define SIGN_COUNT 32
#define LONGEST_SIGN 2
static Keyword signs[] = {
    //2
    {"//",SINGLE_COMMENT_START},{"/*",MULTI_COMMENT_START},

    {">=",GREATER_EQUAL},{"<=",LESS_EQUAL},
    {"||",OR},{"&&",AND},
    {"==",EQUAL_EQUAL},{"!=",BANG_EQUAL},
    //1
    {"+",ADD},{"-",SUB},
    {"*",MUL},{"/",DIV},
    {">",GREATER},{"<",LESS},

    {"=",EQUAL},{";",SEMICOLON},{":",COLON},
    {"?",QUESTION},{"!",EXCLAMATION},

    {"(",ARGUMENT_START},{")",ARGUMENT_END},
    {"[",ARRAY_START},{"]",ARRAY_END},
    {",",COMMA},

    {"&",B_AND},{"|",B_OR},
    {"^",B_XOR},
    {"<<",B_LEFT},{">>",B_RIGHT},
};

//Lexer -----------------------------------------------------------
Node* new_empty_node(MemoryGroup* memory);
void lexer_reset_node(Lexer* lexer);
void lexer_advance(Lexer* lexer);
void lexer_back(Lexer* lexer);
char lexer_peek(Lexer* lexer);
//New Line --------------------------------------------------------
void lexer_new_line_init(Lexer* lexer);
void lexer_add_new_line(Lexer* lexer);
void lexer_count_space(Lexer* lexer);
//String ----------------------------------------------------------
void lexer_new_string(Lexer* lexer);
void lexer_single_string_add_char(Lexer* lexer);
void lexer_string_end(Lexer* lexer);
//Integer ---------------------------------------------------------
void lexer_new_integer(Lexer* lexer);
void lexer_add_digit(Lexer* lexer);
void lexer_add_dot(Lexer* lexer);
void lexer_add_decimal(Lexer* lexer);
//Identifier ------------------------------------------------------
void lexer_new_identifier(Lexer* lexer);
void lexer_add_char(Lexer* lexer);
void lexer_check_identifier(Lexer* lexer);
//Sign ------------------------------------------------------------
void lexer_new_sign(Lexer* lexer);
void lexer_add_char_to_sign(Lexer* lexer);
void lexer_check_sign(Lexer* lexer);
//Comment ---------------------------------------------------------
void lexer_comment_advance(Lexer* lexer);
void lexer_end_comment(Lexer* lexer);
//  Error ----------------------------------------------------------
void lexer_error_decimal(Lexer* lexer);

static lexer_function states[END][LT_END] = {
    [EMPTY] ={
        [LT_SPACE] = lexer_advance,
        [LT_UNDERSCORE] =lexer_new_identifier,
        [LT_SINGLE_QUOTE] = lexer_new_string,
        [LT_DOUBLE_QUOTE] =lexer_new_string,
        [LT_BACKTICK] = lexer_new_string,
        [LT_SIGN] =lexer_new_sign,
        [LT_DOT] =lexer_error_decimal,
        [LT_DIGIT] =lexer_new_integer,
        [LT_ALPHA] =lexer_new_identifier,
        [LT_NEW_LINE] = lexer_new_line_init
    },
    [SINGLE_COMMENT] = {
        [LT_SPACE] = lexer_advance,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_advance,
        [LT_DOUBLE_QUOTE] = lexer_advance,
        [LT_BACKTICK] = lexer_advance,
        [LT_SIGN] = lexer_advance,
        [LT_DOT] = lexer_advance,
        [LT_DIGIT] = lexer_advance,
        [LT_ALPHA] = lexer_advance,
        [LT_NEW_LINE] = lexer_end_comment,
    },
    [MULTI_COMMENT] = {
        [LT_SPACE] = lexer_advance,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_advance,
        [LT_DOUBLE_QUOTE] = lexer_advance,
        [LT_BACKTICK] = lexer_advance,
        [LT_SIGN] = lexer_comment_advance,
        [LT_DOT] = lexer_advance,
        [LT_DIGIT] = lexer_advance,
        [LT_ALPHA] = lexer_advance,
        [LT_NEW_LINE] = lexer_advance,
    },
    [INTEGER] = {
        [LT_SPACE] = lexer_reset_node,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_reset_node,
        [LT_DOUBLE_QUOTE] = lexer_reset_node,
        [LT_BACKTICK] = lexer_reset_node,
        [LT_SIGN] = lexer_reset_node,
        [LT_DOT] = lexer_add_dot,
        [LT_DIGIT] = lexer_add_char,
        [LT_ALPHA] = lexer_reset_node,
        [LT_NEW_LINE] = lexer_reset_node
    },
    [FLOAT] = {
        [LT_SPACE] = lexer_reset_node,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_reset_node,
        [LT_DOUBLE_QUOTE] = lexer_reset_node,
        [LT_BACKTICK] = lexer_reset_node,
        [LT_SIGN] = lexer_reset_node,
        [LT_DOT] = lexer_reset_node,
        [LT_DIGIT] = lexer_add_char,
        [LT_ALPHA] = lexer_reset_node,
        [LT_NEW_LINE] = lexer_reset_node
    },
    [S_STRING] ={
        [LT_SPACE] = lexer_single_string_add_char,
        [LT_UNDERSCORE] = lexer_single_string_add_char,
        [LT_SINGLE_QUOTE] = lexer_single_string_add_char,
        [LT_DOUBLE_QUOTE] = lexer_single_string_add_char,
        [LT_BACKTICK] = lexer_single_string_add_char,
        [LT_SIGN] = lexer_single_string_add_char,
        [LT_DOT] = lexer_single_string_add_char,
        [LT_DIGIT] = lexer_single_string_add_char,
        [LT_ALPHA] = lexer_single_string_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [D_STRING] ={
        [LT_SPACE] = lexer_add_char,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_add_char,
        [LT_DOUBLE_QUOTE] = lexer_string_end,
        [LT_BACKTICK] = lexer_add_char,
        [LT_SIGN] = lexer_add_char,
        [LT_DOT] = lexer_add_char,
        [LT_DIGIT] = lexer_add_char,
        [LT_ALPHA] = lexer_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [B_STRING] = {
        [LT_SPACE] = lexer_add_char,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_add_char,
        [LT_DOUBLE_QUOTE] = lexer_add_char,
        [LT_BACKTICK] = lexer_string_end,
        [LT_SIGN] = lexer_add_char,
        [LT_DOT] = lexer_add_char,
        [LT_DIGIT] = lexer_add_char,
        [LT_ALPHA] = lexer_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [SIGN] = {
        [LT_SIGN] = lexer_add_char_to_sign,
    },
    [IDENTIFIER] = {
        [LT_SPACE] = lexer_check_identifier,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_check_identifier,
        [LT_DOUBLE_QUOTE] = lexer_check_identifier,
        [LT_BACKTICK] = lexer_check_identifier,
        [LT_SIGN] = lexer_check_identifier,
        [LT_DOT] = lexer_check_identifier,
        [LT_DIGIT] = lexer_add_char,
        [LT_ALPHA] = lexer_add_char,
        [LT_NEW_LINE] = lexer_check_identifier
    }, 
    [NEW_LINE] = {
        [LT_SPACE] = lexer_count_space,
        [LT_UNDERSCORE] = lexer_reset_node,
        [LT_SINGLE_QUOTE] = lexer_reset_node,
        [LT_DOUBLE_QUOTE] = lexer_reset_node,
        [LT_BACKTICK] = lexer_reset_node,
        [LT_SIGN] = lexer_reset_node,
        [LT_DOT] = lexer_reset_node,
        [LT_DIGIT] = lexer_reset_node,
        [LT_ALPHA] = lexer_reset_node,
        [LT_NEW_LINE] = lexer_reset_node
    },
};

#endif
