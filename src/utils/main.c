#include "include.h"

#define SHOW_ERROR_FUNCTION 1


static char* VALUE(Token* token){
    if(token->type == T_EMPTY){return "EMPTY";}
    if(token->type == T_INTEGER || token->type == T_NEW_LINE || token->type == T_FLOAT){return STRINGIFY(token->value.integer);}
    return token->value.string;
}
static char* multiply_string(char* str,int times){
    char* new_str = malloc(sizeof(char));
    for(int i=0;i<times;i++){
        new_str = realloc(new_str,sizeof(char)*(strlen(new_str)+strlen(str)+1));
        strcat(new_str,str);
    }
    return new_str;
}
char** ALL_SCOPES(char* scope,size_t* return_size){
    int size = 0;
    char** scopes = SPLIT(scope,"!~!",&size);
    char** new_scopes = malloc(sizeof(char*));
    size_t new_size = 0;
    char* current_scope = scopes[0];
    new_scopes = realloc(new_scopes,sizeof(char*)*(new_size+1));
    new_scopes[new_size++] = current_scope;
    for(int i =1;i<size;i++){
        strcat(current_scope,"!~!");
        strcat(current_scope,scopes[i]);
        new_scopes = realloc(new_scopes,sizeof(char*)*(new_size+1));
        new_scopes[new_size++] = current_scope;
    }
    *return_size = new_size;
    return new_scopes;
}
void ERROR(int condition,int line,char **message,const char* fun,char* scope){
    if(!condition){return;}
    char *msg = malloc(sizeof(char));
    char* space = malloc(sizeof(char));
    for (int i = 0;message[i]!=NULL; i++){
        msg = realloc(msg, sizeof(char) * (strlen(msg) + strlen(message[i])));
        strcat(msg, message[i]);
    }
    msg = realloc(msg, sizeof(char) * (strlen(msg) + 1));
    msg[strlen(msg)] = '\0';
    if(strcmp(scope,"PUBLIC")){
        size_t _ = 0; 
        char* file = ALL_SCOPES(scope,&_)[0];
        int line_size = 0;
        int index = 0;
        char** lines =get_error_lines(file,line,3,&line_size,&index);
        int start_line = line - index;
        printf("%sERROR",RED);
        printf("%s[%s%d%s]",WHITE,MAGENTA,line,WHITE);
        printf("%s[%s%s%s]",WHITE,MAGENTA,scope,WHITE);
        #if SHOW_ERROR_FUNCTION == 1
            printf("%s[%s%s%s]",WHITE,MAGENTA,fun,WHITE);
        #endif
        printf("\n");
        for(int i=0;i<line_size;i++){
            printf("%s|%d\t%s%s%s\n",CYAN,start_line++,WHITE,lines[i],RESET);
            if(i==index){
                int current_line_length = strlen(lines[i]);
                printf("%s",RED);
                printf("\t%s\n",multiply_string("^",current_line_length));
                printf("\t%s\n%s",msg,RESET);
            }
        }
    }
    else{
        printf("%sERROR:\t%s%s\n",RED,msg,RESET);
    }
    exit(1);
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
void PRINT_COMMAND(Command* command,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%sCommand: %s:%d:%s\n",tab,command->declaration==T_NEW_LINE ? "NEW_LINE" : "SEMICOLON",command->spacing,PARSER_TYPE(command->type));
    for(int i=0;i<(int)command->ast_size;i++){PRINT_AST(command->asts[i],level+1);}
    for(int i=0;i<(int)command->command_size;i++){PRINT_COMMAND(command->commands[i],level+1);}
}
void PRINT_TOKEN(Token* token,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%sToken:%d:%s",tab,token->line,TOKEN_TYPE(token->type));
    if(token->type == T_IDENTIFIER || token->type == T_STRING || token->type == T_SIGN || token->type == T_INTEGER || token->type == T_FLOAT){
        printf("%s\tValue:%s",tab,VALUE(token));
    }
    printf("\n");
    for(int i=0;i<token->size;i++){PRINT_TOKEN(token->value.children[i],level+1);}
}
void PRINT_AST(AST* ast,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%sAST:%d:%s",tab,ast->line,PARSER_TYPE(ast->type));
    if(ast->type == P_IDENTIFIER || ast->type == P_INTEGER || ast->type == P_FLOAT){
        char* value;
        if(ast->type == P_EMPTY){value = "EMPTY";}
        else if(ast->type == P_INTEGER || ast->type == P_FLOAT){value = STRINGIFY(ast->value.integer);}
        else{value = ast->value.string;}
        printf("\tValue: %s",value);
    }
    printf("\n");
    for(int i=0;i<ast->size;i++){PRINT_AST(ast->children[i],level+1);}
}
<<<<<<< HEAD
char* PARSER_TYPE(int type){
    return (char*[]){
        "P_EMPTY",
        "P_IDENTIFIER",
        "P_INTEGER",
        "P_FLOAT",

        "P_ADD",
        "P_SUB",
        "P_MUL",
        "P_DIV",

        "P_TYPE",
        "P_POINTER",
        "P_ARRAY",
        "P_ARGUMENT",
        
        "P_VARIABLE_DECLARATION",   
        "P_FUNCTION_DECLARATION",
        "P_FUNCTION_CALL",
        "P_IF","P_ELIF",
        "P_LOOP",
        "P_BREAK","P_CONTINUE",
        "P_RETURN",
=======
void PRINT_BYTECODE(Bytecode* bytecode,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%sBytecode:%s\n",tab,TYPE(bytecode->type));
    for(int i=0;i<bytecode->value_size;i++){
        PRINT_BYTECODE(bytecode->values[i],level+1);
    }
    for(int i=0;i<bytecode->children_size;i++){
        PRINT_BYTECODE(bytecode->children[i],level+1);
    }
}

char* VALUE(void* value,int type){
    if(type == EMPTY){return "EMPTY";}
    if(type == INTEGER || type == NEW_LINE || type == SEMICOLON || type == FUNCTION_START){return STRINGIFY(*(int*)value);}
    if(!strcmp(value, "\n")){return "\\n";}
    if(type >= ACTION_START && type <= ACTION_END){return STRINGIFY(*(int*)value);}
    return (char*)value;
}
char* TYPE(int type){
    return (char*[]){
        "EMPTY","INTEGER","STRING","SYMBOL",
        "IDENTIFIER",
        "ARRAY","ARGUMENT",

        "ACTION_START",
            "PUBLIC","MODULE",
            "FUNCTION","VARIABLE",
            "IF","ELIF",
            "PUSH","PUT",
            "LOOP",
            "BREAK","CONTINUE",
            "PLUS","MINUS",
            "MULTIPLY","DIVIDE",
            "EQUAL_EQUAL", "BANG_EQUAL", // == !=
            "LT", "GT", // < >
            "OR", "AND", // || &&
            "MACRO_REPLACE",
        "ACTION_END",
        "POP",

        "ARGUMENT_START","ARGUMENT_END",
        "FUNCTION_START","FUNCTION_END",
        "ARRAY_START","ARRAY_END",
        "COMMA", // ,
        "SKIP",
        "NEW_LINE","SEMICOLON", // \n ;
>>>>>>> d07e883d9b9063c3ba51823417d248f076526617

        "P_END",
    }[type];
}
char* TOKEN_TYPE(int type){
    return (char*[]){
        "EMPTY","INTEGER","FLOAT","SINGLE STRING","DOUBLE STRING","BACKTICK STRING", 
        "SIGN","COMMENT",
        "IDENTIFIER","NEW_LINE",
        "SEMICOLON","STRING",
        
        "IF","ELIF",
        "LOOP",
        "BREAK","CONTINUE",
        "RETURN",
        
        "PLUS","MINUS",
        "MULTIPLY","DIVIDE",
        "EQUAL_EQUAL", "BANG_EQUAL",
        "LT", "GT", 
        "OR", "AND",
        "EQUAL", 

        "ARGUMENT_START","ARGUMENT_END","ARGUMENT",
        "ARRAY_START","ARRAY_END","ARRAY",
        
        "COMMA",
        
        "TYPE_START",
            "I8","I16","I32","I64",
        "TYPE_END",
        
        "END",
    }[type];
}
