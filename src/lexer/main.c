#include "include.h"
#include "type.h"

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
    }
    return result;
}
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
}
Lexer* new_lexer(CompilerOptions* options){
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->options = options;
    lexer->tokens = malloc(sizeof(Token*));
    lexer->token = new_empty_token(1);
    lexer->token_size = 0;
    lexer->line = 1;
    lexer->column = 0;
    lexer->index = 0;
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
    return lexer;
}
