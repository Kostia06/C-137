#include "include.h"

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
        printf("%s[%s%s%s]\n",WHITE,MAGENTA,fun,WHITE);
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
char* STRINGIFY_CHAR(char value){
    char* str = malloc(sizeof(char));
    sprintf(str,"%c",value);
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
void PRINT_TOKEN(Token* token){
    printf("Token:%d:%d\t\t%s\t\t%s\n",token->line,token->column,VALUE(token->value,token->type),TYPE(token->type)); \
}
void PRINT_BLOCK(NodeBlock* block,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%s%d:Block\t%s\t%s\tsize:%zu\tline:%d\tspacing:%d\n",tab,block->index,TYPE(block->type),block->scope,block->node_size,block->line,block->spacing);
    for(int i=0;i<(int)block->block_size;i++){
        PRINT_BLOCK(block->blocks[i],level+1);
    }
    for(int i=0;i<(int)(block->node_size);i++){
        PRINT_NODE(block->nodes[i],level+1);
    }
}
void PRINT_NODE(Node* node,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%sNode:%d\t%s\t%s\n",tab,node->line,TYPE(node->type),VALUE(node->value,node->type));
    for(int i =0;i<node->node_size;i++){
        PRINT_NODE(node->nodes[i],level+1);
    }
  
}
char* VALUE(void* value,int type){
    if(type == EMPTY){return "EMPTY";}
    if(type == INTEGER || type == NEW_LINE || type == SEMICOLON || type == LBRACE){return STRINGIFY(*(int*)value);}
    return (char*)value;
}
char* TYPE(int type){
    return (char*[]){
        "EMPTY","INTEGER","STRING",
        "IDENTIFIER","BLOCK",
        "ARRAY","ARGUMENT",
        "FUNCTION","PIPE_FUNCTION",

        "KEYWORD_START",
            "FN","ENUM","VAR","USE","STRUCT",
            "CONST","STATIC",
            "IF","ELIF",
            "WHILE","FOR",
            "RETURN","BREAK","CONTINUE",
        "KEYWORD_END",

        "TYPES_START",
            "INT","DOUBLE","FLOAT",
            "VOID","CHAR",
        "TYPES_END",

        "MACRO_REPLACE",
        "MACRO_FUNCTION",
        "MACRO_DEFINE",

        "LPAREN", "RPAREN", // ()
        "LBRACE", "RBRACE", // {}
        "LBRACKET", "RBRACKET", // []
        "COMMA", // ,
        "BANG", // !
        "LT", "GT", // < >
        "LT_EQUAL", "GT_EQUAL", // <= >=
        "BANG_EQUAL", "EQUAL_EQUAL", // != ==
        "PIPE_PIPE", "AMPERSAND_AMPERSAND", // || &&
        "SKIP",
        "NEW_LINE","SEMICOLON", // ; \n
        "PIPE", "AMPERSAND", // | &

        "ACTION_START",
            "PLUS", "MINUS", // + -
            "STAR", "BACKSLASH", // * /
            "CARET", "PERCENT", // ^ %
        "ACTION_END",

        "EQUAL", // =
        
        "END"
    }[type];
}