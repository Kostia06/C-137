#include "include.hpp"

Compiler::Compiler(char** argv, int argc): lexer(nullptr), parser(nullptr){
    error_handler = new ErrorHandler();

    signs = std::vector<Keyword>() = {
        {"::",DOUBLE_COLON},
        {"//",SINGLE_COMMENT_START},
        {"/*",MULTI_COMMENT_START},
        {"==",EQUAL_EQUAL},   {"!=",BANG_EQUAL},
        {">=",GREATER_EQUAL}, {"<=",LESS_EQUAL},
        {"||",OR},            {"&&",AND},
        {"<<",B_LEFT},        {">>",B_RIGHT},
        {"+",ADD},            {"-",SUB},
        {"*",MUL},            {"/",DIV},
        {"%",MOD},            {"^",POW},
        {">",GREATER},        {"<",LESS},           {"=",EQUAL},
        {";",SEMICOLON},      {":",COLON},
        {"?",QUESTION},       {"!",EXCLAMATION},
        {"(",ARG_START},      {")",ARG_END},
        {"[",ARRAY_START},    {"]",ARRAY_END},
        {"{",BLOCK_START},    {"}",BLOCK_END},
        {",",COMMA},          {".",DOT},
        {"'",S_STRING},       {"\"",D_STRING},      {"`",B_STRING}
    };

    keywords = std::vector<Keyword>() = {
        // conditional
        {"if",IF},
        {"else",ELSE},
        {"while",WHILE},
        {"for",FOR},
        // struct
        {"struct",STRUCT},
        {"enum",ENUM},
        {"typedef", TYPEDEF},
        // looping
        {"switch",SWITCH},
        {"case",CASE},
        {"default",DEFAULT},
        // goto
        {"goto",GOTO},
        {"break",BREAK},
        {"continue",CONTINUE},
        {"return",RETURN},
        // types
        {"void",VOID},
        {"i1",I1},    {"i8",I8},
        {"i16",I16},  {"i32",I32},
        {"i64",I64},  {"i128",I128},
        {"f16",F16},  {"f32",F32},
        {"f64",F64},  {"f128",F128}
    };
    for(int i = 1; i < argc; i++){ run(argv[i]); }
}

void Compiler::run(std::string file_name){
    // setting default values
    current_file_name = file_name;
    current_file_content = "\n" + read_file(file_name); // + "\n" so that the first token is indent
    // error
    error_handler->set_file(current_file_name, current_file_content);
    // running lexer
    lexer = new Lexer(this);
    if(error_handler->print_errors()){ return; }
    // running parser
    parser = new Parser(this);
    if(error_handler->print_errors()){ return; }
    //deleting
    delete lexer;
    delete parser;
}


Compiler::~Compiler(){
    delete error_handler;
}





