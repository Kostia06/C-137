#include "include.h"

#define KEYWORD_SIZE        18

static char* keyword_config_names[KEYWORD_SIZE] = {
    "_function","_enumeration","_variable", "_structure",
    "_constant","_static",
    "_if", "_else_if", 
    "_while", "_for",
    "_return", "_break", "_continue",

    "_integer","_double","_float",
    "_void","_character"
};
static char* keyword_names[KEYWORD_SIZE] = {
    "fn","enum","var","struct",
    "const","static",
    "if", "elif", 
    "while", "for",
    "return", "break", "continue",

    "int","double","float",
    "void","char"
};
static int keyword_types[KEYWORD_SIZE] = {
    FN,ENUM,VAR,STRUCT,
    CONST,STATIC,
    IF, ELIF, 
    WHILE,FOR,
    RETURN, BREAK, CONTINUE,

    INT,DOUBLE,FLOAT,
    VOID,CHAR,
};
#define MACRO_SIZE      3

static char* macro_names[MACRO_SIZE] = {
    "replace","function","define",
};
static int macro_types[MACRO_SIZE] = {
    MACRO_REPLACE,MACRO_FUNCTION,MACRO_DEFINE,
};

static int is_digit(char c){return (c >= '0' && c <= '9');}
static int is_string(char c){return c == '\"' || c == '\''|| c == '`';}
static int is_alpha(char c){return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');}
static int is_alphanumeric(char c){return is_digit(c) || is_alpha(c) || c=='_';}
static int is_directory(char c){return is_digit(c) || is_alpha(c) || c == '_' || c== '/'|| c=='.' ;}

static void lexer_advance(Lexer* lexer){
    lexer->current_char = lexer->text[lexer->index++];
    lexer->column++;
    if(lexer->current_char == '\n'){
        lexer->line++;
        lexer->column = 0;
    }
}
static char lexer_peek(Lexer* lexer){
    if(lexer->index >= lexer->text_size){return '\0';}
    return lexer->text[lexer->index];
}
static int lexer_keyword_type(char* word){
    char* new_word = LOWER(word);
    for(int i = 0;i < KEYWORD_SIZE;i++){
        if(!strcmp(new_word,keyword_names[i])){return keyword_types[i];}
    }
    return IDENTIFIER;
}
static int lexer_macro_type(char* word,Lexer* lexer){
    char* new_word = LOWER(word);
    for(int i = 0;i < MACRO_SIZE;i++){
        if(!strcmp(new_word,macro_names[i])){return macro_types[i];}
    }
    ERROR(1,lexer->token->line,(char*[]){"Macro \"",word,"\" doesn't exist",NULL},__func__,lexer->current_file);
    return EMPTY;
}
static int lexer_sign_type(char c,int line,char* scope){
    switch(c){
        case '+':return PLUS;
        case '-':return MINUS;
        case '*':return STAR;
        case '/':return BACKSLASH;
        case '^':return CARET;
        case '%':return PERCENT;
        case '=':return EQUAL;
        case '(':return LPAREN;
        case ')':return RPAREN;
        case '[':return LBRACKET;
        case ']':return RBRACKET;
        case '{':return LBRACE;
        case '}':return RBRACE;
        case '!':return BANG;
        case ',':return COMMA;
        case '<':return LT;
        case '>':return GT;
        case '&':return AMPERSAND;
        case '|':return PIPE;
        case ';':return SEMICOLON;
        case '\\':return SKIP;
        default:{
            ERROR(1,line, (char *[]){"Unexpected character \"",STRINGIFY_CHAR(c),"\"",NULL},__func__,scope);
        }
    }
    return EOF;
}

static void lexer_integer(Lexer* lexer){
    lexer->token->type = INTEGER;
    int value = 0;
    while(is_digit(lexer->current_char) || lexer->current_char == '_' || lexer->current_char == '.'){
        if(lexer->current_char == '_'){
            lexer_advance(lexer);
            continue;
        }
        if(lexer->current_char == '.'){
            if(!is_digit(lexer_peek(lexer))){break;}
            lexer_advance(lexer);
            float decimal = 0;
            int decimal_len = 0;
            while(is_digit(lexer->current_char)){
                decimal = decimal * 10 + (lexer->current_char - '0');
                decimal_len++;
                lexer_advance(lexer);
            }
            value += decimal / pow(10, decimal_len);
            continue;
        }
        value = value * 10 + (lexer->current_char - '0');
        lexer_advance(lexer);
    }
    value = lexer->negative ? value * -1 : value;
    lexer->negative = 0;
    lexer->token->value = malloc(sizeof(int));
    *(int*)lexer->token->value = value;
}
static void lexer_string(Lexer* lexer){
    lexer->token->type = STRING;
    char* replace_sequences = "\"\'nrt0";
    char* replace_colors = "012345678";
    char *result = malloc(sizeof(char));
    int len = 0;
    char end = lexer->current_char;
    lexer_advance(lexer);
    while(end != lexer->current_char && lexer->index < lexer->text_size){
        result = realloc(result, sizeof(char) * (len + 1));
        result[len++] = lexer->current_char;
        lexer_advance(lexer);
        if(len>0 && strchr(replace_colors,lexer->current_char)!=NULL&&result[len-1]=='%'){
            char* value;
            switch(lexer->current_char){
                case '0':value = WHITE;break;
                case '1':value = RED;break;
                case '2':value = YELLOW;break;
                case '3':value = GREEN;break;
                case '4':value = BLUE;break;
                case '5':value = CYAN;break;
                case '6':value = MAGENTA;break;
                case '7':value = BLACK;break;
                case '8':value = RESET;break;
            }
            result[len-1] = '\0';
            result = realloc(result, sizeof(char) * (len + strlen(value)));
            strcat(result,value);
            lexer_advance(lexer);
        }
        if(len>0 && strchr(replace_sequences, lexer->current_char) != NULL && result[len-1] == '\\'){
            switch(lexer->current_char){
                case 'n':result[len-1] = '\n';break;
                case 't':result[len-1] = '\t';break;
                case 'r':result[len-1] = '\r';break;
                case '0':result[len-1] = '\0';break;
                case '\'':result[len-1] = '\'';break;
                case '\"':result[len-1] = '\"';break;
            }
            lexer_advance(lexer);
        }
    }
    result = realloc(result, sizeof(char) * (len + 1));
    result[len] = '\0';
    lexer->token->value = result;
    if(lexer->current_char != end){
        ERROR(1,lexer->token->line, (char*[]){"Unmatched ",STRINGIFY_CHAR(end),NULL},__func__,lexer->current_file);
    }
    lexer_advance(lexer);
}
static void lexer_keyword(Lexer* lexer){
    char *result = malloc(sizeof(char));
    int len = 0;
    while (is_alphanumeric(lexer->current_char)){
        result = realloc(result, sizeof(char) * (len + 1));
        result[len++] = lexer->current_char;
        lexer_advance(lexer);
    }
    result = realloc(result, sizeof(char) * (len + 1));
    result[len] = '\0';
    lexer->token->type = lexer_keyword_type(result);
    lexer->token->value = result;
}
static void lexer_function(Lexer* lexer){
    lexer_advance(lexer);
    char *result = malloc(sizeof(char));
    int len = 0;
    while (is_alphanumeric(lexer->current_char)){
        result = realloc(result, sizeof(char) * (len + 1));
        result[len++] = lexer->current_char;
        lexer_advance(lexer);
    }
    lexer->token->type = PIPE_FUNCTION;
    lexer->token->value = result;
}
static void lexer_sign(Lexer* lexer){
    lexer->token->type = lexer_sign_type(lexer->current_char, lexer->token->line,lexer->current_file);
    char* value = malloc(sizeof(char) * 3);
    value[0] = lexer->current_char;
    value[1] = '\0';
    lexer_advance(lexer);
    switch(lexer->token->type){
        case EQUAL:{
            if(lexer->current_char == '='){
                lexer->token->type = EQUAL_EQUAL;
                value[1] = '=';value[2] = '\0';
                lexer_advance(lexer);
            }
            break;
        }
        case LT:{
            if(lexer->current_char == '='){
                lexer->token->type = LT_EQUAL;
                value[1] = '=';value[2] = '\0';
                lexer_advance(lexer);
            }
            break;
        }
        case GT:{
            if(lexer->current_char == '='){
                lexer->token->type = LT_EQUAL;
                value[1] = '=';value[2] = '\0';
                lexer_advance(lexer);
            }
            break;
        }
        case AMPERSAND:{
            if(lexer->current_char == '&'){
                lexer->token->type = AMPERSAND_AMPERSAND;
                value[1] = '&';value[2] = '\0';
                lexer_advance(lexer);
                break;
            }
            ERROR(1,lexer->line, (char*[]){"Ampersand must be followed by another Ampersand, '&'",NULL},__func__,lexer->current_file);
        }
        case PIPE:{
            if(lexer->current_char == '|'){
                lexer->token->type = PIPE_PIPE;
                value[1] = '|';value[2] = '\0';
                lexer_advance(lexer);
                break;
            }
            ERROR(1,lexer->line, (char*[]){"Pipe must be followed by another Pipe, '|'",NULL},__func__,lexer->current_file);
        }
        case BANG:{
            if(lexer->current_char == '='){
                lexer->token->type = BANG_EQUAL;
                value[1] = '=';value[2] = '\0';
                lexer_advance(lexer);
            }
            break;
        }
    }
    lexer->token->value = value;
}
static void lexer_macro(Lexer* lexer){
    lexer_advance(lexer);
    char *result = malloc(sizeof(char));
    int len = 0;
    while (is_alphanumeric(lexer->current_char)){
        result = realloc(result, sizeof(char) * (len + 1));
        result[len++] = lexer->current_char;
        lexer_advance(lexer);
    }
    lexer->token->type =lexer_macro_type(result,lexer);
    lexer->token->value = result;
}

void lex(Lexer* lexer){
    lexer_advance(lexer);
    while(lexer->index <= lexer->text_size){
        lexer->token = malloc(sizeof(Token));
        lexer->token->line = lexer->line;
        lexer->token->column = lexer->column;
        if(lexer->current_char == ' '){lexer_advance(lexer);continue;}
        if(lexer->current_char == '\n'){
            lexer->spacing=0;
            lexer_advance(lexer);
            while(lexer->current_char == ' '){lexer->spacing++;lexer_advance(lexer);}
            if(lexer->last_type == NEW_LINE){
                *(int*)lexer->tokens[lexer->token_size-1]->value = lexer->spacing;
                continue;
            }
            lexer->token->type = NEW_LINE;
            lexer->token->value = malloc(sizeof(int));
            *(int*)lexer->token->value = lexer->spacing;
            goto add_token;
        }
        if(is_digit(lexer->current_char)){lexer_integer(lexer);goto add_token;}
        if(is_string(lexer->current_char)){lexer_string(lexer);goto add_token;}
        if(is_alpha(lexer->current_char)){lexer_keyword(lexer);goto add_token;}
        if(lexer->current_char == '.'){lexer_function(lexer);goto add_token;}
        if(lexer->current_char == '#'){lexer_macro(lexer);goto add_token;}
        lexer_sign(lexer);
        if(lexer->token->type == SKIP){lexer_advance(lexer);continue;}
        if(lexer->token->type == SEMICOLON || lexer->token->type == LBRACE){
            lexer->token->value = malloc(sizeof(int));
            *(int*)lexer->token->value = lexer->spacing+1;
        }
        goto add_token;
        add_token:
            lexer->tokens = realloc(lexer->tokens, sizeof(Token*) * (lexer->token_size + 1));
            lexer->tokens[lexer->token_size++] = lexer->token;
            lexer->last_type = lexer->token->type;
    }
    ERROR(lexer->last_type != NEW_LINE,lexer->line,(char *[]){"End of file should end with a new line",NULL},__func__,lexer->current_file);
}   
Lexer* new_lexer(HashTable* table,Config* config){
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->token = malloc(sizeof(Token));
    lexer->tokens = malloc(sizeof(Token*));
    lexer->token_size = 0;
    lexer->line = 1;
    lexer->column = 0;
    lexer->negative = 0;
    lexer->index = 0;
    lexer->spacing = 0;
    lexer->current_char = '\0';
    lexer->table = table;
    lexer->last_type = EMPTY;
    if(config->configured){
        ERROR(!check_config(CONFIG_FILE),0,(char *[]){"Config file not found",NULL},__func__,"PUBLIC");
        ERROR(!check_config_table(CONFIG_FILE,"config"),0,(char *[]){"Table in the config file is not found",NULL},__func__,"PUBLIC");
        if(check_config_table(CONFIG_FILE,"keywords")){
            for(int i=0;i<KEYWORD_SIZE;i++){
                if(check_config_key(CONFIG_FILE,"keywords",keyword_config_names[i])){
                    keyword_names[i] = read_config(CONFIG_FILE,"keywords",keyword_config_names[i]);
                }
            }
        }
    }
    lexer->text = read_file(config->input_file);
    lexer->text_size = strlen(lexer->text);
    lexer->current_file = config->input_file;
    return lexer;
}