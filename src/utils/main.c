#include "include.h"

char* SYNC(char** array){
    int num = 0;
    int length = 0;
    while (array[num] != NULL) {
        length += strlen(array[num++]);
    }
    char* combined = (char*)malloc((length + 1) * sizeof(char));
    int index = 0;
    for (int i = 0; i < num; i++) {
        strcpy(combined + index, array[i]);
        index += strlen(array[i]);
    }
    combined[length] = '\0';
    return combined;
}
char* READ_FILE(ErrorGroup* error,MemoryGroup* memory,char* file_name){
    FILE* file = fopen(file_name, "r");
    if(!file){
        char* message = SYNC((char*[]){"File not found",NULL});
        error_single_init(error,FILE_ERROR,0,0,message);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);
    char* buffer =  mem_init(memory,file_size+1);
    size_t butes_read = fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}
char* STRINGIFY(float num){
    float decimal = num - (int)num;
    if (decimal == 0.0) {
        char* str = (char*)malloc(12 * sizeof(char));
        snprintf(str, 12, "%d", (int)num);
        return str;
    }
    char* str = (char*)malloc(32 * sizeof(char)); 
    snprintf(str, 32, "%f", num);
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
// table shows if node has a value with memory allocated
static int has_memory[END] = {
    [STRING] = 1,   [IDENTIFIER] = 1,
    [INTEGER] = 1,  [FLOAT] = 1,
};
void PRINT_NODE(Node* node,int level){
    char* tab = malloc(sizeof(char)*level);
    for(int i=0;i<level;i++){tab[i] = '\t';}
    printf("%sNODE:%d:",tab,!node->children?-1:(int)node->children->size);
    printf("%s",PRINT_TYPE(node->type));
    if(node->type == STRING || node->type == IDENTIFIER || node->type == INTEGER || node->type == FLOAT){
        printf("\t\t");
        char* value;
        if(node->type == EMPTY){value = "EMPTY";}
        else{value = node->value.string;}
        printf("Value: %s",value);
    }
    printf("\n");
    if(!node->children){return;}
    for(int i=0;i<node->children->size;i++){PRINT_NODE(vector_get(node->children,i),level+1);} 
}
// free a node
void FREE_NODE(MemoryGroup* memory,Node* node){
    if(has_memory[node->type]){mem_free(memory,node->value.string);}
    if(node->children){
        for(int i=0;i<(int)node->children->size;i++){
            FREE_NODE(memory,vector_get(node->children,i));
        }
        mem_free(memory,node->children);
    }
    mem_free(memory,node);
}
// count how much memory used
int NODE_MEMORY(Node* node){
    int count = 1;
    count += has_memory[node->type];
    if(node->children){
        count++;
        for(int i=0;i<(int)node->children->size;i++){
            count += NODE_MEMORY(vector_get(node->children,i));
        }
    }
    return count; 
}
char* PRINT_TYPE(int type){
    return (char*[]){
        //Types
        "EMPTY", "INTEGER", "FLOAT", "STRING", 
        "ARRAY","ARGUMENT","BLOCK",
        "IDENTIFIER",

        // Keywords + Command names
        "FUNCTION", "TYPE", "EXPRESSION", "SIGN", "STRUCT", 
        "MACRO",
        "DECLARATION","FUNCTION CALL",
        "IF","ELSE IF" ,"ELSE", "LOOP",
        "BREAK", "CONTINUE", "RETURN",
        // Macros
        "MACRO START", 
            "MACRO IF", "MACRO ELSE IF", "MACRO ELSE",
            "MACRO SWAP", 
            "MACRO END",
        "MACRO END",
        // Types
        "I1","I8","I16","I32", "I64", "I128",
        "F16","F32", "F64", "F128", 

        // Operators
        "OP START",
            "ADD", "SUB", "MUL", "DIV",
            "EQUAL_EQUAL", "BANG_EQUAL",
            "GREATER", "LESS", "GREATER_EQUAL", "LESS_EQUAL",
            "OR", "AND",

            "BITWISE AND", "BITWISE OR", 
            "BITWISE XOR",
            "BITWISE LEFT SHIFT", "BITWISE RIGHT SHIFT",
        "OP END",
        // Signs
        
        "ARGUMENT_START", "ARGUMENT_END",
        "ARRAY_START", "ARRAY_END",
        "BLOCK_START", "BLOCK_END",

        "NEW_LINE", "SEMICOLON",
        "QUESTION", "EXCLAMATION",
        
        "S_STRING", "D_STRING", "B_STRING",

        "EQUAL","COMMA","COLON",
        
        "SINGLE_COMMENT", "SINGLE_COMMENT_START",
        "MULTI_COMMENT","MULTI_COMMENT_START",

        "SYNTAX_ERROR", "FILE_ERROR", "MEMORY_ERROR",

    }[type];
}
