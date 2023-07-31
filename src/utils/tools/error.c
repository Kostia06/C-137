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
ErrorGroup* error_group_init(Compiler* compiler){
    ErrorGroup* error = malloc(sizeof(ErrorGroup)); 
    error->rules = vector_init(compiler->memory);
    error->scope = compiler->scope;
    return error;
}
// checks if there is any errors
void error_execute(ErrorGroup* error){
    for(int i=0;i<error->rules->size;i++){
        ErrorRule* rule = vector_get(error->rules,i);
        switch(rule->type){
            case FILE_ERROR:{
                printf("%s%s: %sfile error: %s%s%s\n",WHITE,error->scope,RED,WHITE,rule->message,RESET);
                break;
            }
            case MEMORY_ERROR:{
                printf("%s%s:%smemory error: %s%s%s\n",WHITE,error->scope,RED,WHITE,rule->message,RESET);
                break;
            }
            case SYNTAX_ERROR:{
                int error_line;
                char* error_message = get_error(error->scope,RED,rule->start,rule->end,&error_line);
                printf(
                    "%s%s:%ssyntax error: %s%s in line %d\n",
                    WHITE,
                    error->scope,
                    RED,
                    WHITE,
                    rule->message,
                    error_line
                );
                printf("%s\n",error_message);
                break; 
            }
        }
    }
    if(error->rules->size > 0){exit(1);}
}
// add a error to the error group
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
}
