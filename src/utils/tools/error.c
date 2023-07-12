#include "../include.h"

// get the line of the error
static int fine_line(char* file_name, int index){
    FILE* file = fopen(file_name,"r");
    int line = 1;
    char c;
    while((c = fgetc(file)) != EOF){
        if(c == '\n'){line++;}
        if(index == 0){break;}
        index--;
    }
    return line;
}
// get the error from the file
static char* get_error(char* file_name,char* error_color, int start, int end,int* error_line){
    FILE* file = fopen(file_name,"r");
    char c;
    char* error = malloc(sizeof(char)+1);
    *error_line = fine_line(file_name,start);
    int line = 1,i=0,start_error = 0,end_error = 0;
    while((c = getc(file)) != EOF){
        if(*error_line == line && !start_error){
            char* new_line = SYNC((char*[]){CYAN,STRINGIFY(line),"|\t",NULL});
            strcat(error,new_line);
            strcat(error,WHITE);
            start_error = 1;
        }
        if(i == start){strcat(error,error_color);}
        else if(i == end){
            strcat(error,"\033[0m");
            strcat(error,WHITE);end_error = 1;
        }
        if(start_error){
            char* result = malloc(sizeof(char)*2);
            result[0] = c;
            result[1] = '\0';
            strcat(error,result);
        }
        // new line
        if(c == '\n'){
            if(end_error){break;}
            if(start_error){
                char* new_line = SYNC((char*[]){CYAN,STRINGIFY(line),"|\t",NULL});
                strcat(error,RESET);
                strcat(error,new_line);
                strcat(error,error_color);
            }
            line++;
        }
        i++;
    }
    strcat(error,RESET);
    strcat(error,"\0");
    fclose(file);
    return error;

}
// split the lines
static char** split_lines(char* str,char* split){
    char* string = malloc(sizeof(char)*strlen(string)+1);
    strcat(string,str);
    char* token = strtok(string,split);
    char** array = malloc(sizeof(char*));
    int size = 0;
    while(token != NULL){
        array = realloc(array,sizeof(char*)*(size+1));
        array[size++] = token;
        token = strtok(NULL,split);
    }
    array = realloc(array,sizeof(char*)*(size+1));
    array[size] = NULL;
    return array;
}
ErrorGroup* error_group_init(char* scope){
    ErrorGroup* error = malloc(sizeof(ErrorGroup));
    error->rules = vector_init();
    error->scope = scope;
    return error;
}








void error_execute(ErrorGroup* error){
    for(int i=0;i<error->rules->size;i++){
        ErrorRule* rule = vector_get(error->rules,i);
        switch(rule->type){
            case FILE_ERROR:{
                printf("%s%s: %sfile error: %s%s%s\n",WHITE,error->scope,RED,WHITE,rule->message,RESET);
                break;
            }
            case SYNTAX_ERROR:{
                int error_line;
                char* error_message = get_error(error->scope,RED,rule->start,rule->end,&error_line);
                printf(
                    "%s%s: %ssyntax error: %s%s\n",
                    WHITE,
                    error->scope,
                    RED,
                    WHITE,
                    rule->message
                );
                printf("%s\n",error_message);
                break; 
            }
        }
    }
    if(error->rules->size > 0){exit(1);}
}







void error_single_init(ErrorGroup* error,int type, int start, int end, char* message){
    ErrorRule* rule = malloc(sizeof(ErrorRule));
    rule->type = type;
    rule->start = start;
    rule->end = end;
    rule->message = message;
    vector_add(error->rules,rule);
}
void error_free(ErrorGroup* error){
    vector_clear(error->rules);
    free(error->rules);
    free(error);
}
