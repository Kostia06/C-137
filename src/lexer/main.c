#include "include.h"
typedef struct{
    char* config_name,*name;
    int type;
}Keyword;

#define KEYWORD_SIZE        17
static Keyword keywords[KEYWORD_SIZE]={
    {"_function","fn",FUNCTION},
    {"_enumeration","enum",ENUMERATOR},
    {"_variable","var",VARIABLE},
    {"_structure","struct",STRUCTURE},
    {"_if","if",IF},
    {"_else_if","elif",ELIF},
    {"_while","while",WHILE},
    {"_for","for",FOR},
    {"_return","return",RETURN},
    {"_break","break",BREAK},
    {"_continue","continue",CONTINUE},
    {"_i1","i1",I1},
    {"_i8","i8",I8},
    {"_i16","i16",I16},
    {"_i32","i32",I32},
    {"_i64","i64",I64},
    {"_replace","replace",MACRO_REPLACE},
};

#define SIDES_SIZE 22
static Keyword sides[SIDES_SIZE] = {
    {"_add","+",PLUS},
    {"_subtract","-",MINUS},
    {"_multiply","*",STAR},
    {"_divide","/",BACKSLASH},
    {"_power","^",CARET},
    {"_modulus","%",PERCENT},
    {"_argument_start","(",ARGUMENT_START},
    {"_argument_end",")",ARGUMENT_END},
    {"_array_start","[",ARRAY_START},
    {"_array_end","]",ARRAY_END},
    {"_bind_start","{",FUNCTION_START},
    {"_bind_end","}",FUNCTION_END},
    {"_opposite","!",BANG},
    {"_less_than","<",LT},
    {"_greater_than",">",GT},
    {"_less_equal_than","<=",LT_EQUAL},
    {"_greater_equal_than",">=",GT_EQUAL},
    {"_opposite_equal","!=",BANG_EQUAL},
    {"_equal_equal","==",EQUAL_EQUAL},
    {"_or","||",OR},
    {"_and","&&",AND},
    {"_skip","\\",SKIP},
};

#define SEQUENCES_SIZE 5
static char* sequence_start[SEQUENCES_SIZE] = {"\"","'","`","/*","//"};
static char* sequence_end[SEQUENCES_SIZE] = {"\"","'","`","*/","\n"};
static int sequence_type[SEQUENCES_SIZE] = {STRING,STRING,STRING,EMPTY,EMPTY};

static int is_digit(char c){return (c >= '0' && c <= '9');}
static int is_string(char c){return c == '\"' || c == '\''|| c == '`';}
static int is_alpha(char c){return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');}
static int is_alphanumeric(char c){return is_digit(c) || is_alpha(c) || c=='_';}

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
static char* lexer_long_peek(Lexer* lexer,int length){
    char* result = malloc(sizeof(char)*length);
    for(int i=0;i<length;i++){
        if(lexer->index-1+i >= lexer->text_size){result[i] = '\0';}
        else{result[i] = lexer->text[lexer->index-1+i];}
    }
    return result;
}

static int compare_keywords(const void* a, const void* b){
    Keyword* a1 = (Keyword*)a;
    Keyword* b1 = (Keyword*)b;
    return strlen(b1->name)-strlen(a1->name);
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
    lexer->token->value = malloc(sizeof(int));
    *(int*)lexer->token->value = value;
}
static void lexer_sequence(Lexer* lexer,char* end,int type){
    lexer->token->type = type;
    char* replace_sequences = "\"\'nrt0";
    char *result = malloc(sizeof(char));
    int len = 0, end_len = strlen(end),found =0;
    
    lexer_advance(lexer);
    while(lexer->index < lexer->text_size){
        char* peek = lexer_long_peek(lexer,end_len);
        if(!strcmp(peek,end)){
            for(int i=0;i<end_len;i++){lexer_advance(lexer);}
            found = 1;
            free(peek);
            break;
        }
        free(peek);
        result = realloc(result, sizeof(char) * (len + 1));
        result[len++] = lexer->current_char;
        lexer_advance(lexer);
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
    ERROR(!found,lexer->token->line, (char*[]){"Unmatched ",end,NULL},__func__,lexer->current_file);
    
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
    lexer->token->value = result;
    for(int i=0;i<KEYWORD_SIZE;i++){
        if(!strcmp(result, keywords[i].name)){
            lexer->token->type = keywords[i].type;return;
        }
    }
    lexer->token->type = IDENTIFIER;
}
static void lexer_comment(Lexer* lexer){
    if(lexer_peek(lexer) == '/'){
        while(lexer->current_char != '\n'){lexer_advance(lexer);}
    }
    else if(lexer_peek(lexer)=='*'){
        while(lexer->index <= lexer->text_size && (lexer->current_char != '*' || lexer_peek(lexer) != '/')){lexer_advance(lexer);}
        lexer_advance(lexer);
        lexer_advance(lexer);
    }
}
static void lexer_sign(Lexer* lexer){
    int long_peek_len = 0;
    char* long_peek;
    for(int i=0;i<SIDES_SIZE;i++){
        int len = strlen(sides[i].name);
        if(long_peek_len!=len){free(long_peek);long_peek = lexer_long_peek(lexer, len);}
        if(!strncmp(long_peek, sides[i].name, len)){
            lexer->token->type = sides[i].type;
            lexer->token->value = malloc(sizeof(char)*len);
            lexer->token->value = sides[i].name;
            for(int j=0;j<len;j++){lexer_advance(lexer);}
            return;
        }
    }
    ERROR(1,lexer->token->line, (char*[]){"Unexpected character ",STRINGIFY_CHAR(lexer->current_char),NULL},__func__,lexer->current_file);
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
        for(int i=0;i<SEQUENCES_SIZE;i++){
            char* peek = lexer_long_peek(lexer, strlen(sequence_start[i]));
            if(!strcmp(peek, sequence_start[i])){
                free(peek);
                lexer_sequence(lexer,sequence_end[i],sequence_type[i]);
                goto add_token;
            }
        }
        if(is_alpha(lexer->current_char) || lexer->current_char=='_'){lexer_keyword(lexer);goto add_token;}
        lexer_sign(lexer);
        if(lexer->token->type == SKIP){lexer_advance(lexer);continue;}
        if(lexer->token->type == SEMICOLON || lexer->token->type == FUNCTION_START){
            lexer->token->value = malloc(sizeof(int));
            *(int*)lexer->token->value = lexer->spacing+1;
        }
        goto add_token;
        add_token:
            if(lexer->token->type == EMPTY){continue;}
            lexer->tokens = realloc(lexer->tokens, sizeof(Token*) * (lexer->token_size + 1));
            lexer->tokens[lexer->token_size++] = lexer->token;
            lexer->last_type = lexer->token->type;
    }
    ERROR(lexer->last_type != NEW_LINE,lexer->line,(char *[]){"End of file should end with a new line",NULL},__func__,lexer->current_file);
}   
Lexer* new_lexer(Config* config){
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->token = malloc(sizeof(Token));
    lexer->tokens = malloc(sizeof(Token*));
    lexer->token_size = 0;
    lexer->line = 1;
    lexer->column = 0;
    lexer->index = 0;
    lexer->spacing = 0;
    lexer->current_char = '\0';
    lexer->last_type = EMPTY;
    qsort(keywords, KEYWORD_SIZE, sizeof(Keyword), compare_keywords);
    qsort(sides,SIDES_SIZE, sizeof(Keyword), compare_keywords);
    lexer->text = read_file(config->input_file);
    lexer->text_size = strlen(lexer->text);
    lexer->current_file = config->input_file;
    return lexer;
}
