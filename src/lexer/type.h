#ifndef LEXER_TYPE_H
#define LEXER_TYPE_H

typedef void(*lexer_function)(Lexer*);
enum{
    LT_SPACE,
    LT_UNDERSCORE,
    LT_SINGLE_QUOTE,
    LT_DOUBLE_QUOTE,
    LT_BACKTICK,
    LT_SIGN,
    LT_DOT, 
    LT_SLASH,
    LT_DIGIT,
    LT_SEMICOLON,
    LT_ALPHA,
    LT_SKIP,
    LT_NEW_LINE,
}LexerType;
int characters[256] = {
    0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, LT_NEW_LINE, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	LT_SPACE, LT_SIGN, LT_DOUBLE_QUOTE, LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN,LT_SINGLE_QUOTE,
	LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_SIGN, LT_DOT, LT_SLASH,
	LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT, LT_DIGIT,
	LT_DIGIT, LT_DIGIT, LT_SIGN, LT_SEMICOLON,LT_SIGN ,LT_SIGN, LT_SIGN, LT_SIGN,
	LT_SIGN, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_SIGN, LT_SKIP, LT_SIGN,LT_SIGN, LT_UNDERSCORE,
	LT_BACKTICK, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_ALPHA,
	LT_ALPHA, LT_ALPHA, LT_ALPHA, LT_SIGN, LT_SIGN ,LT_SIGN, LT_SIGN,0,
};

typedef struct{
    char* name;
    int type;
}Keyword;

#define KEYWORD_COUNT 14
Keyword keywords[] = {
    {"if",T_IF},
    {"elif",T_ELIF},
    {"loop",T_LOOP},
    {"break",T_BREAK},
    {"return",T_RETURN},
    {"continue",T_CONTINUE},
    {"i8",T_I8},
    {"i16",T_I16},
    {"i32",T_I32},
    {"i64",T_I64},
};
#define SIGN_COUNT 17
Keyword signs[] = {
    {"+",T_ADD},
    {"-",T_SUB},
    {"*",T_MUL},
    {"/",T_DIV},
    {"==",T_EQUAL_EQUAL},
    {"!=",T_BANG_EQUAL},
    {">",T_GT},
    {"<",T_LT},
    {"||",T_OR},
    {"&&",T_AND},
    {"=",T_EQUAL},
    {";",T_SEMICOLON},
    {"(",T_ARGUMENT_START},
    {")",T_ARGUMENT_END},
    {"[",T_ARRAY_START},
    {"]",T_ARRAY_END},
    {",",T_COMMA},
};

Token** format_tokens(Token** tokens,size_t size,char* scope,size_t* return_size);

#endif
