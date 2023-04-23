#include "include.h"
<<<<<<< HEAD
#include "type.h"
=======

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
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617

// keyword -----------------------------------------------------------------------------------------
static int compare_keyword(const void* a,const void* b){
    Keyword* keyword_a = (Keyword*)a;
    Keyword* keyword_b = (Keyword*)b;
    return strlen(keyword_b->name) - strlen(keyword_a->name);
}
// Lexer -------------------------------------------------------------------------------------------
static Token* new_empty_token(int line){
    Token* token = malloc(sizeof(Token));
    token->type = T_EMPTY;
    token->line = line;
    return token;
}
static void lexer_reset_token(Lexer* lexer){
    lexer->tokens = realloc(lexer->tokens,sizeof(Token*)*(lexer->token_size+1));   
    lexer->tokens[lexer->token_size++] = lexer->token;
    lexer->token = new_empty_token(lexer->line);
}
static void lexer_advance(Lexer* lexer){
    lexer->index++;
    lexer->column++;
    lexer->current_char = lexer->text[lexer->index];
}
//new line -----------------------------------------------------------------------------------------
static void lexer_new_line(Lexer* lexer){
    lexer->token->type = T_NEW_LINE;
    lexer->token->value.integer = 0;
    lexer->line++;
    lexer->column = 0;
    lexer_advance(lexer);
}
static void lexer_add_new_line(Lexer* lexer){
    lexer->token->value.string = realloc(lexer->token->value.string,sizeof(char)*(strlen(lexer->token->value.string)+2));
    lexer->token->value.string[strlen(lexer->token->value.string)] = '\n';
    lexer->column = 0;
    lexer->line++;
    lexer_advance(lexer);
}
static void lexer_count_space(Lexer* lexer){
    lexer->token->value.integer++;
    lexer_advance(lexer);
}
//string  -----------------------------------------------------------------------------------------
static void lexer_new_string(Lexer* lexer){
    char c = lexer->current_char;
    lexer->token->type = c == '`' ? T_B_STRING : c == '"' ? T_D_STRING : T_S_STRING;
    lexer->token->value.string = malloc(sizeof(char)*2);
    lexer_advance(lexer);    
}
//integer   -----------------------------------------------------------------------------------------
static void lexer_new_integer(Lexer* lexer){
    lexer->token->type = T_INTEGER;
    lexer->token->value.integer = lexer->current_char - '0';
    lexer_advance(lexer);
}
static void lexer_add_digit(Lexer* lexer){
    lexer->token->value.integer = lexer->token->value.integer * 10 + lexer->current_char - '0';
    lexer_advance(lexer);
}
static void lexer_add_dot(Lexer* lexer){
    lexer->token->type = T_FLOAT;
    lexer->token->size = 1;
    lexer_advance(lexer);
}
static void lexer_add_decimal(Lexer* lexer){
    lexer->token->value.integer += (lexer->current_char - '0') / pow(10,lexer->token->size);
    lexer->token->size++;
    lexer_advance(lexer);
}
//identifier -----------------------------------------------------------------------------------------
static void lexer_new_identifier(Lexer* lexer){
    lexer->token->type = T_IDENTIFIER;    
    lexer->token->value.string = malloc(sizeof(char)*2);
    lexer->token->value.string[0] = lexer->current_char;
    lexer_advance(lexer);
}
static void lexer_add_char(Lexer* lexer){
    lexer->token->value.string = realloc(lexer->token->value.string,sizeof(char)*(strlen(lexer->token->value.string)+2));
    lexer->token->value.string[strlen(lexer->token->value.string)] = lexer->current_char;
    lexer_advance(lexer);
}
static void lexer_check_identifier(Lexer* lexer){
    for(int i = 0;i < KEYWORD_COUNT;i++){
        if(strcmp(lexer->token->value.string,keywords[i].name) == 0){
            lexer->token->type = keywords[i].type;
            break;
        }
    }
    lexer_reset_token(lexer);
}
// sign -----------------------------------------------------------------------------------------------
static int single_sign_types[T_END] ={
    [T_ARGUMENT_END] = 1,
    [T_ARGUMENT_START] = 1,
    [T_ARRAY_END] = 1,
    [T_ARRAY_START] = 1,
    [T_SEMICOLON] = 1,
    [T_COMMA] = 1,
    [T_ADD] = 1,
    [T_SUB] = 1,
    [T_MUL] = 1, 
    [T_DIV] = 1,
};
static void lexer_new_sign(Lexer* lexer){
    lexer->token->type = T_SIGN;
    lexer->token->value.string = malloc(sizeof(char)*2);
}
static void lexer_add_char_to_sign(Lexer* lexer){
    lexer_add_char(lexer);
    for(int i = 0;i < SIGN_COUNT;i++){
        if(!strcmp(lexer->token->value.string,signs[i].name)){
            int function = single_sign_types[signs[i].type];
            if(function){
                lexer->token->type = signs[i].type;
                lexer_reset_token(lexer);
                return;
            }
            break;
        }
    }
}
static void lexer_skip(Lexer* lexer){
    lexer_advance(lexer);lexer_advance(lexer);
}
<<<<<<< HEAD
static void lexer_semicolon(Lexer* lexer){
    lexer->token->type = T_SEMICOLON;
    lexer_advance(lexer);lexer_reset_token(lexer);
}
static void lexer_check_sign(Lexer* lexer){
    for(int i = 0;i < SIGN_COUNT;i++){
        if(!strcmp(lexer->token->value.string,signs[i].name)){
            lexer->token->type = signs[i].type;
            break;
        }
    }
    lexer_reset_token(lexer);
}
// Comment -----------------------------------------------------------------------------------------------
static void lexer_comment(Lexer* lexer){
    lexer_advance(lexer);
    if(lexer->current_char == '/'){lexer->token->type = T_COMMENT;}
    else{lexer->token->type = T_SIGN;}
}
static void lexer_end_comment(Lexer* lexer){
    lexer->token->type = T_EMPTY;
}
// Error -----------------------------------------------------------------------------------------------
void lexer_error_decimal(Lexer* lexer){
    ERROR(1,lexer->line,(char*[]){"Decimal point without digits",NULL},__func__,lexer->current_file);
}


static lexer_function states[10][13] = {
    [T_EMPTY] ={
        [LT_SPACE] = lexer_advance,
        [LT_UNDERSCORE] =lexer_new_identifier,
        [LT_SINGLE_QUOTE] = lexer_new_string,
        [LT_DOUBLE_QUOTE] =lexer_new_string,
        [LT_BACKTICK] = lexer_new_string,
        [LT_SLASH] = lexer_comment,
        [LT_SIGN] =lexer_new_sign,
        [LT_DOT] =lexer_error_decimal,
        [LT_DIGIT] =lexer_new_integer,
        [LT_SEMICOLON] =lexer_semicolon,
        [LT_ALPHA] =lexer_new_identifier,
        [LT_SKIP] =lexer_skip,
        [LT_NEW_LINE] = lexer_new_line
    },
    [T_COMMENT] = {
        [LT_SPACE] = lexer_advance,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_advance,
        [LT_DOUBLE_QUOTE] = lexer_advance,
        [LT_BACKTICK] = lexer_advance,
        [LT_SLASH] = lexer_advance,
        [LT_SIGN] = lexer_advance,
        [LT_DOT] = lexer_advance,
        [LT_DIGIT] = lexer_advance,
        [LT_SEMICOLON] = lexer_advance,
        [LT_ALPHA] = lexer_advance,
        [LT_SKIP] = lexer_advance,
        [LT_NEW_LINE] = lexer_end_comment,
    },
    [T_INTEGER] = {
        [LT_SPACE] = lexer_reset_token,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_reset_token,
        [LT_DOUBLE_QUOTE] = lexer_reset_token,
        [LT_BACKTICK] = lexer_reset_token,
        [LT_SLASH] = lexer_reset_token,
        [LT_SIGN] = lexer_reset_token,
        [LT_DOT] = lexer_add_dot,
        [LT_DIGIT] = lexer_add_digit,
        [LT_SEMICOLON] = lexer_reset_token,
        [LT_ALPHA] = lexer_reset_token,
        [LT_SKIP] = lexer_reset_token,
        [LT_NEW_LINE] = lexer_reset_token
    },
    [T_FLOAT] = {
        [LT_SPACE] = lexer_reset_token,
        [LT_UNDERSCORE] = lexer_advance,
        [LT_SINGLE_QUOTE] = lexer_reset_token,
        [LT_DOUBLE_QUOTE] = lexer_reset_token,
        [LT_BACKTICK] = lexer_reset_token,
        [LT_SLASH] = lexer_reset_token,
        [LT_SIGN] = lexer_reset_token,
        [LT_DOT] = lexer_reset_token,
        [LT_DIGIT] = lexer_add_decimal,
        [LT_SEMICOLON] = lexer_reset_token,
        [LT_ALPHA] = lexer_reset_token,
        [LT_SKIP] = lexer_reset_token,
        [LT_NEW_LINE] = lexer_reset_token
    },
    [T_S_STRING] ={
        [LT_SPACE] = lexer_add_char,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_reset_token,
        [LT_DOUBLE_QUOTE] = lexer_add_char,
        [LT_BACKTICK] = lexer_add_char,
        [LT_SLASH] = lexer_add_char,
        [LT_SIGN] = lexer_add_char,
        [LT_DOT] = lexer_add_char,
        [LT_DIGIT] = lexer_add_char,
        [LT_SEMICOLON] = lexer_add_char,
        [LT_ALPHA] = lexer_add_char,
        [LT_SKIP] = lexer_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [T_D_STRING] ={
        [LT_SPACE] = lexer_add_char,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_add_char,
        [LT_DOUBLE_QUOTE] = lexer_reset_token,
        [LT_BACKTICK] = lexer_add_char,
        [LT_SLASH] = lexer_add_char,
        [LT_SIGN] = lexer_add_char,
        [LT_DOT] = lexer_add_char,
        [LT_DIGIT] = lexer_add_char,
        [LT_SEMICOLON] = lexer_add_char,
        [LT_ALPHA] = lexer_add_char,
        [LT_SKIP] = lexer_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [T_B_STRING] = {
        [LT_SPACE] = lexer_add_char,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_add_char,
        [LT_DOUBLE_QUOTE] = lexer_add_char,
        [LT_BACKTICK] = lexer_reset_token,
        [LT_SLASH] = lexer_add_char,
        [LT_SIGN] = lexer_add_char,
        [LT_DOT] = lexer_add_char,
        [LT_DIGIT] = lexer_add_char,
        [LT_SEMICOLON] = lexer_add_char,
        [LT_ALPHA] = lexer_add_char,
        [LT_SKIP] = lexer_add_char,
        [LT_NEW_LINE] = lexer_add_new_line
    },
    [T_SIGN] = {
        [LT_SPACE] = lexer_check_sign,
        [LT_UNDERSCORE] = lexer_check_sign,
        [LT_DOUBLE_QUOTE] = lexer_check_sign,
        [LT_SINGLE_QUOTE] = lexer_check_sign,
        [LT_BACKTICK] = lexer_check_sign,
        [LT_SLASH] = lexer_check_sign,
        [LT_SIGN] = lexer_add_char_to_sign,
        [LT_DOT] = lexer_check_sign,
        [LT_DIGIT] = lexer_check_sign,
        [LT_SEMICOLON] = lexer_check_sign,
        [LT_ALPHA] = lexer_check_sign,
        [LT_SKIP] = lexer_check_sign,
        [LT_NEW_LINE] = lexer_check_sign
    },
    [T_IDENTIFIER] = {
        [LT_SPACE] = lexer_check_identifier,
        [LT_UNDERSCORE] = lexer_add_char,
        [LT_SINGLE_QUOTE] = lexer_check_identifier,
        [LT_DOUBLE_QUOTE] = lexer_check_identifier,
        [LT_BACKTICK] = lexer_check_identifier,
        [LT_SLASH] = lexer_check_identifier,
        [LT_SIGN] = lexer_check_identifier,
        [LT_DOT] = lexer_check_identifier,
        [LT_DIGIT] = lexer_add_char,
        [LT_SEMICOLON] = lexer_check_identifier,
        [LT_ALPHA] = lexer_add_char,
        [LT_SKIP] = lexer_check_identifier,
        [LT_NEW_LINE] = lexer_check_identifier
    }, 
    [T_NEW_LINE] = {
        [LT_SPACE] = lexer_count_space,
        [LT_UNDERSCORE] = lexer_reset_token,
        [LT_SINGLE_QUOTE] = lexer_reset_token,
        [LT_DOUBLE_QUOTE] = lexer_reset_token,
        [LT_BACKTICK] = lexer_reset_token,
        [LT_SLASH] = lexer_reset_token,
        [LT_SIGN] = lexer_reset_token,
        [LT_DOT] = lexer_reset_token,
        [LT_DIGIT] = lexer_reset_token,
        [LT_SEMICOLON] = lexer_reset_token,
        [LT_ALPHA] = lexer_reset_token,
        [LT_SKIP] = lexer_reset_token,
        [LT_NEW_LINE] = lexer_reset_token
    },
};

static Token* slit_tokens(Token** tokens,int size,size_t line,int target,char* scope){
    Token* result = new_empty_token(line);
    Token* token = new_empty_token(line);    
    Token** hold = malloc(sizeof(Token*));
    size_t hold_size = 0;
    for(int i=0;i<(int)size;i++){
        Token* current = tokens[i];
        if(i+1 == size){
            hold = realloc(hold,sizeof(Token*)*(hold_size+1));
            hold[hold_size++] = current;
        }
        if(current->type == target || i+1==size){
            token->value.children = hold;
            token->size = hold_size;
            result->value.children = realloc(result->value.children,sizeof(Token)*(result->size+1));
            result->value.children[result->size++] = token;
            token = new_empty_token(line);
            hold = malloc(sizeof(Token*));
            hold_size = 0;
        }
        else{
            hold = realloc(hold,sizeof(Token*)*(hold_size+1));
            hold[hold_size++] = current;
        }
=======

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
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
    }
    replaces[id] = replace;
}
<<<<<<< HEAD
static Token* loop_tokens(Token** tokens,size_t size,int index,char* scope,int start,int end,int *return_index){
    Token* token = malloc(sizeof(Token));
    Token** hold = malloc(sizeof(Token*));
    size_t hold_size = 0;
    int count = 1;
    int line = tokens[index++]->line;
    for(;index<(int)size;index++){
        Token* current = tokens[index];
        if(current->type == start){count++;}
        if(current->type == end){count--;}
        if(!count){break;}
        hold = realloc(hold,sizeof(Token*)*(hold_size+1));
        hold[hold_size++] = current;
    }
    ERROR(count!=0,line,(char*[]){"Unmatched \"",TOKEN_TYPE(end),"\"",NULL},__func__,scope);
    hold = format_tokens(hold,hold_size,scope,&hold_size);
    token->value.children = hold;
    token->size = hold_size;
    *return_index = index;
    return token;
}

Token** format_tokens(Token** tokens,size_t size,char*scope,size_t* return_size){
    Token** new_tokens = malloc(sizeof(Token*));    
    size_t new_size = 0;
    int index = 0;
    while(index<(size)){
        Token* current = tokens[index];
        new_tokens = realloc(new_tokens,sizeof(Token*)*(new_size+1));
        switch(current->type){
            case T_ARGUMENT_START:{
                Token* token = loop_tokens(tokens,size,index,scope,T_ARGUMENT_START,T_ARGUMENT_END,&index);
                token = slit_tokens(token->value.children,token->size,token->line,T_COMMA,scope);
                token->type = T_ARGUMENT;
                token->line = current->line;
                new_tokens[new_size++] = token;
                break;
            }
            case T_ARRAY_START:{
                Token* token = loop_tokens(tokens,size,index,scope,T_ARRAY_START,T_ARRAY_END,&index);
                token = slit_tokens(token->value.children,token->size,token->line,T_COMMA,scope);
                token->type = T_ARRAY;
                token->line = current->line;
                new_tokens[new_size++] = token;
                break;
            }
            default:{
                new_tokens[new_size++] = current;
                break;
            }
        }
        index++;
    }
    *return_size = new_size;
    return new_tokens;
=======
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
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
}
Lexer* new_lexer(CompilerOptions* options){
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->options = options;
<<<<<<< HEAD
    lexer->tokens = malloc(sizeof(Token*));
    lexer->token = new_empty_token(1);
=======
    lexer->hold = new_parent();
    lexer->tokens = malloc(sizeof(Token*));
    lexer->token = malloc(sizeof(Token));
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
    lexer->token_size = 0;
    lexer->line = 1;
    lexer->column = 0;
    lexer->index = 0;
<<<<<<< HEAD
    lexer->text = read_file(options->file,&lexer->text_size);
    lexer->current_file = options->file;
    lexer->current_char = lexer->text[lexer->index];
    qsort(keywords,KEYWORD_COUNT,sizeof(Keyword),compare_keyword);
    qsort(signs,SIGN_COUNT,sizeof(Keyword),compare_keyword);
    ERROR(lexer->current_char != '\n',1, (char*[]){"File must start with a new line",NULL},__func__,lexer->current_file);
    ERROR(lexer->text[lexer->text_size-1] != '\n',0, (char*[]){"File must end with a new line",NULL},__func__,lexer->current_file);
    while(lexer->index < lexer->text_size){
        int type = characters[lexer->current_char];
        lexer_function function = states[lexer->token->type][type];
        function(lexer);
    }
    ERROR(lexer->token->type != T_NEW_LINE,lexer->token->line, (char*[]){"String not closed",NULL},__func__,lexer->current_file);
    lexer_reset_token(lexer);
    lexer->tokens = format_tokens(lexer->tokens,lexer->token_size,lexer->current_file,&lexer->token_size);
=======
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
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617
    return lexer;
}
