#include "include.h"

typedef struct{
    size_t size;
    Token** tokens;
}Replace;
typedef struct{
    char *name;
    int type;
}Keyword;

Replace* replaces[MAX_HASH_SIZE];

#define KEYWORD_SIZE        13
static Keyword keywords[KEYWORD_SIZE]={
    {"pub",PUBLIC},
    {"fn",FUNCTION},
    {"var",VARIABLE},
    {"if",IF},
    {"elif",ELIF},
    {"loop",LOOP},
    {"push",PUSH},
    {"put",PUT},
    {"pop",POP},
    {"break",BREAK},
    {"continue",CONTINUE},
    {"replace",MACRO_REPLACE},
    {"module",MODULE},
};

#define SIDES_SIZE 19
static Keyword sides[SIDES_SIZE] = {
    {"+",PLUS},
    {"-",MINUS},
    {"*",MULTIPLY},
    {"/",DIVIDE},
    {"(",ARGUMENT_START},
    {")",ARGUMENT_END},
    {"[",ARRAY_START},
    {"]",ARRAY_END},
    {"{",FUNCTION_START},
    {"}",FUNCTION_END},
    {"<",LT},
    {">",GT},
    {"!=",BANG_EQUAL},
    {"==",EQUAL_EQUAL},
    {"||",OR},
    {"&&",AND},
    {"\\",SKIP},
    {",",COMMA},
    {";",SEMICOLON},
};


#define SEQUENCES_SIZE 4
static char sequence_start[SEQUENCES_SIZE] = {'\"','\'','`','~'};
static char sequence_end[SEQUENCES_SIZE] = {'\"','\'','`','\n'};
static int sequence_type[SEQUENCES_SIZE] = {STRING,STRING,STRING,EMPTY};

static int is_whitespace(char c){return c == ' ' || c == '\n';}
static int is_string(char c){return c == '\"' || c == '\'' || c == '`' || c == '~';}
static int is_digit(char c){return (c >= '0' && c <= '9');}
static int is_alpha(char c){return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');}
static int is_alphanumeric(char c){return is_digit(c) || is_alpha(c) || c=='_';}
static int is_symbol(char c){return !is_alphanumeric(c) && !is_string(c) && !is_digit(c) && !is_whitespace(c);}

static Token* new_parent(){
    Token* token = malloc(sizeof(Token));
    int* value = malloc(sizeof(int));
    *value = 0;
    token->value = value;
    token->size = 0;
    return token;
}

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

static void lexer_replace(Lexer* lexer){
    Token* token = lexer->hold;
    ERROR(token->size < 2,token->line,(char*[]){"Macro replace must have at least 2 tokens",NULL},__func__,lexer->current_file);
    Token* name = token->children[0];
    ERROR(name->type != IDENTIFIER && name->type != SYMBOL,name->line,(char*[]){"Macro replace name must be an identifier or symbol",NULL},__func__,lexer->current_file);
    int id = hash_id((char*)name->value);
    Replace* replace = malloc(sizeof(Replace));
    for(int i=1;i<token->size;i++){
        replace->tokens = realloc(replace->tokens,sizeof(Token*)*(replace->size+1));
        replace->tokens[replace->size++] = token->children[i];
    }
    replaces[id] = replace;
}
static void lexer_integer(Lexer* lexer){
    lexer->token->type = INTEGER;
    int value = 0;
    while(is_digit(lexer->current_char) || lexer->current_char == '_' || lexer->current_char == '.'){
        if(lexer->current_char == '_'){lexer_advance(lexer);continue;}
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
static void lexer_sequence(Lexer* lexer,char end,int type){
    lexer->token->type = type;
    char* replace_sequences = "\"\'nrt0";
    char *result = malloc(sizeof(char));
    int len=0,found =0;
    lexer_advance(lexer);
    while(lexer->index < lexer->text_size){
        if(lexer->current_char == end){
            found = 1;
            lexer_advance(lexer);
            break;
        }
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
    char* error = malloc(sizeof(char)*2);
    error[0] = end;
    error[1] = '\0';
    ERROR(!found,lexer->token->line, (char*[]){"Unmatched ",error,NULL},__func__,lexer->current_file);

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
            int type = keywords[i].type;
            lexer->token->type = type;
            if(lexer->hold->type != MACRO_REPLACE && type >= ACTION_START && type <= ACTION_END){
                ERROR(lexer->hold->type != EMPTY,lexer->token->line, (char*[]){"Unexpected action",NULL},__func__,lexer->current_file);
                lexer->hold->type = type;
                lexer->hold->line = lexer->token->line;
                lexer->token->type = EMPTY;
            }
            return;
        }
    }
    lexer->token->type = IDENTIFIER;
    if(lexer->options->macro == 0){return;}
    int id = hash_id(result);
    int line = lexer->token->line;
    Replace* replace_token = replaces[id];
    if(replace_token==NULL){return;}
    for(int i=0;i<(int)replace_token->size;i++){
        Token* token = malloc(sizeof(Token));
        token->type = replace_token->tokens[i]->type;   
        token->value = replace_token->tokens[i]->value; 
        token->line = line;
        lexer->hold->children = realloc(lexer->hold->children, sizeof(Token*) * (lexer->hold->size + 1));
        lexer->hold->children[lexer->hold->size++] = token;
        if(token->type >= ACTION_START && token->type <= ACTION_END){
            ERROR(lexer->hold->type != EMPTY,lexer->token->line, (char*[]){"Unexpected action",NULL},__func__,lexer->current_file);    
            lexer->hold->type = token->type;
        }
        lexer->last_type= token->type;
    }
    lexer->token->type = EMPTY;
}
static void lexer_sign(Lexer* lexer){
    char *result = malloc(sizeof(char));
    int len = 0;
    while (is_symbol(lexer->current_char)){
        result = realloc(result, sizeof(char) * (len + 1));
        result[len++] = lexer->current_char;
        lexer_advance(lexer);
    }
    result = realloc(result, sizeof(char) * (len + 1));
    result[len] = '\0';
    lexer->token->value = result;
    for(int i=0;i<SIDES_SIZE;i++){
        if(!strcmp(result, sides[i].name)){
            lexer->token->type = sides[i].type;return;
        }
    }
    lexer->token->type = SYMBOL;
    if(lexer->options->macro == 0){return;}
    int id = hash_id(result);
    int line = lexer->token->line;
    Replace* replace_token = replaces[id];
    if(replace_token==NULL){return;}
    for(int i=0;i<(int)replace_token->size;i++){
        Token* token = malloc(sizeof(Token));
        token->type = replace_token->tokens[i]->type;   
        token->value = replace_token->tokens[i]->value; 
        token->line = line;
        lexer->hold->children = realloc(lexer->hold->children, sizeof(Token*) * (lexer->hold->size + 1));
        lexer->hold->children[lexer->hold->size++] = token;
        if(token->type >= ACTION_START && token->type <= ACTION_END){
            ERROR(lexer->hold->type != EMPTY,lexer->token->line, (char*[]){"Unexpected action",NULL},__func__,lexer->current_file);    
            lexer->hold->type = token->type;
        }
        lexer->last_type= token->type;
    }
    lexer->token->type = EMPTY;
}

void lex(Lexer* lexer){
    lexer_advance(lexer);
    while(lexer->index <= lexer->text_size){
        lexer->token = malloc(sizeof(Token));
        lexer->token->line = lexer->line;
        if(lexer->current_char == ' '){lexer_advance(lexer);continue;}
        if(lexer->current_char == '\n' || lexer->current_char == ';'){
            if(lexer->hold->type == MACRO_REPLACE && lexer->current_char == ';'){lexer_advance(lexer);continue;}
            int space; 
            if(lexer->current_char == ';'){space = lexer->spacing++;}
            else{space = lexer->spacing;lexer->spacing = 0;}
            lexer_advance(lexer);
            while(lexer->current_char == ' '){lexer->spacing++;lexer_advance(lexer);}
            if(lexer->hold->type == EMPTY && lexer->hold->size == 0){continue;}
            ERROR(lexer->hold->type == EMPTY,lexer->token->line, (char*[]){"Unexpected new line",NULL},__func__,lexer->current_file);
            if(lexer->hold->type == MACRO_REPLACE){
                lexer_replace(lexer);
                lexer->hold = new_parent();
                continue;
            }
            *(int*)lexer->hold->value = space;
            lexer->tokens = realloc(lexer->tokens, sizeof(Token*) * (lexer->token_size + 1)); 
            lexer->tokens[lexer->token_size++] = lexer->hold;
            lexer->hold = new_parent();

        }
        if(is_digit(lexer->current_char)){lexer_integer(lexer);goto add_token;}
        for(int i=0;i<SEQUENCES_SIZE;i++){
            if(lexer->current_char == sequence_start[i]){
                lexer_sequence(lexer,sequence_end[i],sequence_type[i]);
                goto add_token;
            }
        }
        if(is_alpha(lexer->current_char) || lexer->current_char=='_'){lexer_keyword(lexer);goto add_token;}
        lexer_sign(lexer);
        if(lexer->token->type == SKIP){lexer_advance(lexer);continue;}
        if(lexer->token->type == FUNCTION_START || lexer->token->type == FUNCTION_END){
            lexer->tokens = realloc(lexer->tokens, sizeof(Token*) * (lexer->token_size + 1));
            lexer->tokens[lexer->token_size++] = lexer->token;
            continue;
        }
        goto add_token;
        add_token:
            if(lexer->token->type == EMPTY){free(lexer->token->value);free(lexer->token);continue;}
            lexer->hold->children = realloc(lexer->hold->children, sizeof(Token*) * (lexer->hold->size + 1));
            lexer->hold->children[lexer->hold->size++] = lexer->token;
            lexer->last_type = lexer->token->type;
    }
}
Lexer* new_lexer(CompilerOptions* options){
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->options = options;
    lexer->hold = new_parent();
    lexer->tokens = malloc(sizeof(Token*));
    lexer->token = malloc(sizeof(Token));
    lexer->token_size = 0;
    lexer->line = 1;
    lexer->column = 0;
    lexer->index = 0;
    lexer->spacing = 0;
    lexer->current_char = '\0';
    lexer->last_type = EMPTY;
    lexer->text = read_file(options->file,&lexer->text_size);
    lexer->current_file = options->file;
    ERROR(lexer->text[0] != '\n',0, (char*[]){"File must start with a new line",NULL},__func__,lexer->current_file);
    ERROR(lexer->text[lexer->text_size-1] != '\n',0, (char*[]){"File must end with a new line",NULL},__func__,lexer->current_file);
    for(int i=0;i<lexer->text_size;i++){
        printf("%c\n",lexer->text[i]);
    }
    return lexer;
}
