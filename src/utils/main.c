#include "include.h"

#define SHOW_ERROR_FUNCTION 1

static int loop_count = 0;

static char* get_file_from_scope(char* scope){
    int i = 0;
    return SPLIT(scope,"!~!",&i)[0];
}
static char* multiply_string(char* str,int times){
    char* new_str = malloc(sizeof(char));
    for(int i=0;i<times;i++){
        new_str = realloc(new_str,sizeof(char)*(strlen(new_str)+strlen(str)+1));
        strcat(new_str,str);
    }
    return new_str;
}
void ERROR_LOOP(int max){
    if(loop_count++ > max){
        printf("%sERROR%s[%s%d%s][%s%s%s][%s%s%s]\n",RED,WHITE,MAGENTA,0,WHITE,MAGENTA,"LOOP",WHITE,MAGENTA,"LOOP",WHITE);
        exit(1);
    }
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
        char* file = get_file_from_scope(scope);
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
                printf("\t%s\n",msg);
            }
        }
    }
    else{
        printf("%sERROR:\t%s%s\n",RED,msg,RESET);
    }
    exit(1);
}
char* SINGLE_STRING(char**array){
    char* str = malloc(sizeof(char));
    for(int i=0;array[i]!=NULL;i++){
        str = realloc(str,sizeof(char)*(strlen(str)+strlen(array[i])+1));
        strcat(str,array[i]);
    }
    str = realloc(str,sizeof(char)*(strlen(str)+1));
    str[strlen(str)] = '\0';
    return str;
}
char* STRINGIFY(int value){
    char* str = malloc(sizeof(char));
    sprintf(str,"%d",value);
    return str;
}
char* LOWER(char* string){
    char* str = malloc(sizeof(char)*(strlen(string)+1));
    strcpy(str,string);
    for(int i=0;i<(int)strlen(str);i++){
        str[i] = tolower(str[i]);
    }
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
void PRINT_TOKEN(Token* token,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%sToken:%d:%s",tab,token->line,TYPE(token->type));
    if(token->type != ARGUMENT && token->type != EMPTY && token->type != ARRAY){
        printf("%s\tValue:%s",tab,VALUE(token->value,token->type));
    }
    printf("\n");
    for(int i=0;i<token->size;i++){
        PRINT_TOKEN(token->children[i],level+1);
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
        "EMPTY","INTEGER","STRING",
        "IDENTIFIER",
        "ARRAY","ARGUMENT",

        "ACTION_START",
            "PUBLIC","MODULE",
            "FUNCTION","VARIABLE",
            "IF","ELIF",
            "PUSH",
            "LOOP",
            "RETURN","BREAK","CONTINUE",
            "PLUS","MINUS",
            "MULTIPLY","DIVIDE",
        "ACTION_END",
        "POP",
        "MACRO_REPLACE",

        "ARGUMENT_START","ARGUMENT_END",
        "FUNCTION_START","FUNCTION_END",
        "ARRAY_START","ARRAY_END",
        "COMMA", // ,
        "BANG", //!
       "LT", "GT", // < >
        "LT_EQUAL", "GT_EQUAL", // <= >=
        "BANG_EQUAL", "EQUAL_EQUAL", // != ==
        "OR", "AND", // || &&
        "SKIP",
        "NEW_LINE","SEMICOLON", // \n ;

        "END",
    }[type];
}
