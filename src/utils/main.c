#include "include.h"
#include "../vector/include.h"

static char* VALUE(Token* token){
    if(token->type == T_EMPTY){return "EMPTY";}
    if(token->type == T_INTEGER || token->type == T_NEW_LINE || token->type == T_FLOAT){return STRINGIFY(token->value.integer);}
    return token->value.string;
}

char** ALL_SCOPES(char* scope,size_t* return_size){
    int size = 0;
    char** scopes = SPLIT(scope,"!~!",&size);
    char** new_scopes = malloc(sizeof(char*)*size);
    char current_scope[strlen(scopes[0])];
    strcpy(current_scope, scopes[0]);
    for(int i =0;i<size;i++){
        char* result = malloc(sizeof(char)*strlen(current_scope)+1);
        strcat(result,current_scope);
        new_scopes[i] = result;
        if(i+1<size){
            strcat(current_scope,"!~!");
            strcat(current_scope,scopes[i+1]);
        }
    }
    *return_size = size;
    return new_scopes;
}
char* READ_FILE(char* file_name){
    FILE* file = fopen(file_name, "r");
    ERROR(file == NULL,0,0,0,(char*[]){"File not found",NULL},"ERROR");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    char* buffer = (char*) malloc(file_size+1);
    size_t butes_read = fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}
char* STRINGIFY(float value){
    char* str = malloc(sizeof(char));
    sprintf(str,"%f",value);
    return str;
}
char** SPLIT(char* string,char* split,int* return_size){
    char* str = malloc(sizeof(char)*(strlen(string)+1));
    strcpy(str,string);
    char** array = malloc(sizeof(char*));
    char* token = strtok(str,split);
    int i = 0;
    while(token != NULL){
        array = realloc(array,sizeof(char*)*(i+1));
        array[i] = malloc(sizeof(char)*(strlen(token)+1));
        strcpy(array[i],token);
        token = strtok(NULL,split);
        i++;
    }
    array = realloc(array,sizeof(char*)*(i+1));
    array[i] = NULL;
    *return_size = i;
    return array;
}
void PRINT_TOKEN(Token* token){
    printf("Token:%d:%d:%d:%s",token->line,token->column,token->size,TOKEN_TYPE(token->type));
    if(token->type == T_NEW_LINE || token->type == T_IDENTIFIER || token->type == T_S_STRING || token->type == T_STRING || token->type == T_SIGN || token->type == T_INTEGER || token->type == T_FLOAT){
        printf("\tValue:%s",VALUE(token));
    }
    printf("\n");
}
void PRINT_AST(AST* ast,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%sAST:%d:",tab,ast->line);
    if(ast->type == ast->actual_type){printf("%s",PARSER_TYPE(ast->type));}
    else{printf("%s->%s",PARSER_TYPE(ast->actual_type),PARSER_TYPE(ast->type));}
    if(ast->type == P_IDENTIFIER || ast->type == P_INTEGER || ast->type == P_FLOAT){
        char* value;
        if(ast->type == P_EMPTY){value = "EMPTY";}
        else if(ast->type == P_INTEGER || ast->type == P_FLOAT){value = STRINGIFY(ast->value.integer);}
        else{value = ast->value.string;}
        printf("\tValue: %s",value);
    }
    printf("\n");
    if(ast->children == NULL){return;}
    for(int i=0;i<ast->children->size;i++){PRINT_AST(vector_get(ast->children,i),level+1);}
    
}
char* PARSER_TYPE(int type){
    return (char*[]){
        "EMPTY",
        "IDENTIFIER",
        "NEW LINE",

        "COMMA","END OF ARGUMENT",

        "PARAMETERS",
        "TUPLE WITH TYPES",
        "TUPLE WITH VALUES",
        "TUPLE WITH IDENTIFIERS",
       
        "TYPE AND IDENTIFIER",

        "ARRAY","POINTER",

        "INTEGER","FLOAT","STRING",

        "FUNCTION","STRUCT","EQUAL","SEMICOLON",
        "IF","ELIF",
        "LOOP",
        "BREAK","CONTINUE",
        "RETURN",
        
        "EXPRESSION","VARIABLE",

        "ADD","SUBTRACT","MULTIPLY","DIVIDE","MODULO",
        "EQUAL EQUAL","NOT EQUAL", 
        "LESS THAN","GREATER THAN","LESS THAN EQUAL","GREATER THAN EQUAL",
        "OR","AND",

        "TYPE",
        "MUTABLE",
        "I1","I8","I16","I32","I64","F128",
        "F16","F32","F64","F128",

        "END",
    }[type];
}
char* TOKEN_TYPE(int type){
    return (char*[]){
        "EMPTY","INTEGER","FLOAT","SINGLE STRING","DOUBLE STRING","BACKTICK STRING", 
        "SIGN","COMMENT",
        "IDENTIFIER","NEW LINE",
        "STRING",
       
        "FUNCTION","STRUCT","EQUAL","SEMICOLON",
        "IF","ELIF",
        "LOOP",
        "BREAK","CONTINUE",
        "RETURN",
 
        "MUTABLE",

        "PLUS","MINUS",
        "MULTIPLY","DIVIDE",
        "MODULO",

        "EQUAL EQUAL","NOT EQUAL", 
        "LESS THAN","GREATER THAN","LESS THAN EQUAL","GREATER THAN EQUAL",

        "OR", "AND",

        "POINTER",

        "ARGUMENT START","ARGUMENT END","ARGUMENT",
        "ARRAY START","ARRAY END","ARRAY",
        
        "COMMA",
        
        "I1","I8","I16","I32","I64","I128",
        "F16","F32","F64","F128",
        
        "END",
    }[type];
}
