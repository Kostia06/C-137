#ifndef LEXER_PRIVATE_H
#define LEXER_PRIVATE_H

#include "../utils/include.h"
#include "../vector/include.h"

typedef struct{
    Token *token;
    char current_char,*text,*scope;
    size_t token_size,index,text_size;
    size_t line,column;
    Vector* tokens;
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
    LT_SEMICOLON,
    LT_ALPHA,
    LT_HASHTAG,
    LT_NEW_LINE,
    LT_END,
}LexerType;
static int characters[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, LT_NEW_LINE, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	LT_SPACE, LT_SIGN, LT_DOUBLE_QUOTE, LT_HASHTAG, LT_SIGN, LT_SIGN, LT_SIGN,LT_SINGLE_QUOTE,
    LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_DOT, LT_SIGN,
	LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT,
	LT_DIGIT, LT_DIGIT, LT_SIGN, LT_SEMICOLON,LT_SIGN ,LT_SIGN, LT_SIGN, LT_SIGN,
	LT_SIGN, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_SIGN, LT_SIGN, LT_SIGN,LT_SIGN, LT_UNDERSCORE,
	LT_BACKTICK, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_SIGN, LT_SIGN ,LT_SIGN, LT_SIGN,0,
};
#define KEYWORD_COUNT 19
static Keyword keywords[] = {
    {"fn",T_FN},{"struct",T_STRUCT},
    {"if",T_IF},{"elif",T_ELIF},
    {"loop",T_LOOP},
    {"break",T_BREAK},{"continue",T_CONTINUE},
    {"return",T_RETURN},{"mut",T_MUTABLE},
    {"i1",T_I1},{"i8",T_I8},
    {"i16",T_I16},{"i32",T_I32},
    {"i64",T_I64},{"i128",T_I128},
    {"f16",T_F16},{"f32",T_F32},
    {"f64",T_F64},{"f128",T_F128},
};
#define SIGN_COUNT 21
static Keyword signs[] = {
    {"+",T_ADD},{"-",T_SUB},
    {"*",T_MUL},{"/",T_DIV},
    {"^",T_MOD},
    {"==",T_EQUAL_EQUAL},
    {"!=",T_BANG_EQUAL},
    {">",T_GT},{"<",T_LT},
    {">=",T_GT_EQUAL},{"<=",T_LT_EQUAL},
    {"||",T_OR},{"&&",T_AND},
    {"=",T_EQUAL},{"->",T_POINTER},
    {";",T_SEMICOLON},
    {"(",T_ARGUMENT_START},{")",T_ARGUMENT_END},
    {"[",T_ARRAY_START},{"]",T_ARRAY_END},
    {",",T_COMMA},
};

//Lexer -----------------------------------------------------------
Token* new_empty_token(int line);
void lexer_reset_token(Lexer* lexer);
void lexer_advance(Lexer* lexer);
//New Line --------------------------------------------------------
void lexer_new_line(Lexer* lexer);
void lexer_add_new_line(Lexer* lexer);
void lexer_count_space(Lexer* lexer);
//String ----------------------------------------------------------
void lexer_new_string(Lexer* lexer);
void lexer_single_string_add_char(Lexer* lexer);
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
void lexer_skip(Lexer* lexer);
void lexer_semicolon(Lexer* lexer);
void lexer_check_sign(Lexer* lexer);
//Comment ---------------------------------------------------------
void lexer_comment(Lexer* lexer);
void lexer_end_comment(Lexer* lexer);
//  Error ----------------------------------------------------------
void lexer_error_decimal(Lexer* lexer);

static lexer_function states[T_END][LT_END] = {
    [T_EMPTY] ={
        [LT_SPACE] = lexer_advance,
        [LT_UNDERSCORE] =lexer_new_identifier,
        [LT_SINGLE_QUOTE] = lexer_new_string,
        [LT_DOUBLE_QUOTE] =lexer_new_string,
        [LT_BACKTICK] = lexer_new_string,
        [LT_SIGN] =lexer_new_sign,
        [LT_DOT] =lexer_error_decimal,
        [LT_DIGIT] =lexer_new_integer,
        [LT_SEMICOLON] =lexer_semicolon,
        [LT_ALPHA] =lexer_new_identifier,
        [LT_HASHTAG] =lexer_comment,
        [LT_NEW_LINE] = lexer_new_line
    },
    [T_COMMENT] = {
        [LT_SPACE] = lexer_advance,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_advance,
        [LT_DOUBLE_QUOTE] = lexer_advance,
        [LT_BACKTICK] = lexer_advance,
        [LT_SIGN] = lexer_advance,
        [LT_HASHTAG] = lexer_advance,
        [LT_DOT] = lexer_advance,
        [LT_DIGIT] = lexer_advance,
        [LT_SEMICOLON] = lexer_advance,
        [LT_ALPHA] = lexer_advance,
        [LT_NEW_LINE] = lexer_end_comment,
    },
    [T_INTEGER] = {
        [LT_SPACE] = lexer_reset_token,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_reset_token,
        [LT_DOUBLE_QUOTE] = lexer_reset_token,
        [LT_BACKTICK] = lexer_reset_token,
        [LT_SIGN] = lexer_reset_token,
        [LT_HASHTAG] = lexer_reset_token,
        [LT_DOT] = lexer_add_dot,
        [LT_DIGIT] = lexer_add_digit,
        [LT_SEMICOLON] = lexer_reset_token,
        [LT_ALPHA] = lexer_reset_token,
        [LT_NEW_LINE] = lexer_reset_token
    },
    [T_FLOAT] = {
        [LT_SPACE] = lexer_reset_token,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_reset_token,
        [LT_DOUBLE_QUOTE] = lexer_reset_token,
        [LT_BACKTICK] = lexer_reset_token,
        [LT_SIGN] = lexer_reset_token,
        [LT_HASHTAG] = lexer_reset_token,
        [LT_DOT] = lexer_reset_token,
        [LT_DIGIT] = lexer_add_decimal,
        [LT_SEMICOLON] = lexer_reset_token,
        [LT_ALPHA] = lexer_reset_token,
        [LT_NEW_LINE] = lexer_reset_token
    },
    [T_S_STRING] ={
        [LT_SPACE] = lexer_single_string_add_char,
        [LT_UNDERSCORE] = lexer_single_string_add_char,
        [LT_SINGLE_QUOTE] = lexer_single_string_add_char,
        [LT_DOUBLE_QUOTE] = lexer_single_string_add_char,
        [LT_BACKTICK] = lexer_single_string_add_char,
        [LT_SIGN] = lexer_single_string_add_char,
        [LT_HASHTAG] = lexer_single_string_add_char,
        [LT_DOT] = lexer_single_string_add_char,
        [LT_DIGIT] = lexer_single_string_add_char,
        [LT_SEMICOLON] = lexer_single_string_add_char,
        [LT_ALPHA] = lexer_single_string_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [T_D_STRING] ={
        [LT_SPACE] = lexer_add_char,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_add_char,
        [LT_DOUBLE_QUOTE] = lexer_reset_token,
        [LT_BACKTICK] = lexer_add_char,
        [LT_SIGN] = lexer_add_char,
        [LT_HASHTAG] = lexer_add_char,
        [LT_DOT] = lexer_add_char,
        [LT_DIGIT] = lexer_add_char,
        [LT_SEMICOLON] = lexer_add_char,
        [LT_ALPHA] = lexer_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [T_B_STRING] = {
        [LT_SPACE] = lexer_add_char,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_add_char,
        [LT_DOUBLE_QUOTE] = lexer_add_char,
        [LT_BACKTICK] = lexer_reset_token,
        [LT_SIGN] = lexer_add_char,
        [LT_HASHTAG] = lexer_add_char,
        [LT_DOT] = lexer_add_char,
        [LT_DIGIT] = lexer_add_char,
        [LT_SEMICOLON] = lexer_add_char,
        [LT_ALPHA] = lexer_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [T_SIGN] = {
        [LT_SPACE] = lexer_check_sign,
        [LT_UNDERSCORE] = lexer_check_sign,
        [LT_DOUBLE_QUOTE] = lexer_check_sign,
        [LT_SINGLE_QUOTE] = lexer_check_sign,
        [LT_BACKTICK] = lexer_check_sign,
        [LT_SIGN] = lexer_add_char_to_sign,
        [LT_HASHTAG] = lexer_add_char_to_sign,
        [LT_DOT] = lexer_check_sign,
        [LT_DIGIT] = lexer_check_sign,
        [LT_SEMICOLON] = lexer_check_sign,
        [LT_ALPHA] = lexer_check_sign,
        [LT_NEW_LINE] = lexer_check_sign
    },
    [T_IDENTIFIER] = {
        [LT_SPACE] = lexer_check_identifier,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_check_identifier,
        [LT_DOUBLE_QUOTE] = lexer_check_identifier,
        [LT_BACKTICK] = lexer_check_identifier,
        [LT_SIGN] = lexer_check_identifier,
        [LT_HASHTAG] = lexer_check_identifier,
        [LT_DOT] = lexer_check_identifier,
        [LT_DIGIT] = lexer_add_char,
        [LT_SEMICOLON] = lexer_check_identifier,
        [LT_ALPHA] = lexer_add_char,
        [LT_NEW_LINE] = lexer_check_identifier
    }, 
    [T_NEW_LINE] = {
        [LT_SPACE] = lexer_count_space,
        [LT_UNDERSCORE] = lexer_reset_token,
        [LT_SINGLE_QUOTE] = lexer_reset_token,
        [LT_DOUBLE_QUOTE] = lexer_reset_token,
        [LT_BACKTICK] = lexer_reset_token,
        [LT_SIGN] = lexer_reset_token,
        [LT_HASHTAG] = lexer_reset_token,
        [LT_DOT] = lexer_reset_token,
        [LT_DIGIT] = lexer_reset_token,
        [LT_SEMICOLON] = lexer_reset_token,
        [LT_ALPHA] = lexer_reset_token,
        [LT_NEW_LINE] = lexer_reset_token
    },
};

#endif
