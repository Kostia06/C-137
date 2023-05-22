#include "include.h"

static char* multiply_string(char* str,int times){
    char* new_str = malloc(sizeof(char));
    for(int i=0;i<times;i++){
        new_str = realloc(new_str,sizeof(char)*(strlen(new_str)+strlen(str)+1));
        strcat(new_str,str);
    }
    return new_str;
}
static char** get_error_lines(char* file_name,int index,int size,int* return_size,int* return_index){
    char** lines = malloc(size * sizeof(char *)); 
    FILE* file = fopen(file_name, "r");
    int line_size = 0;   
    int* nums = malloc(sizeof(int));
    int num_size = 0;
    for(int i=1;i<=size*2+1;i++){
        int num = index-size+i;
        if(num > 0){
            nums = realloc(nums,sizeof(int)*(num_size+1));
            nums[num_size++] = num;
        }
    }
    int current_line = 1;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL){
        for(int i=0;i<num_size;i++){
            if(current_line == nums[i]){
                lines = realloc(lines,sizeof(char*)*(line_size+1));
                if(buffer[strlen(buffer)-1] == '\n'){buffer[strlen(buffer)-1] = '\0';}
                lines[line_size++] = strdup(buffer);
                if(current_line == index){*return_index = line_size-1;}
                break;
            }
        }
        current_line++;
    }
    *return_size = line_size;
    return lines;
}


void ERROR(int condition, int line, int column,int size, char** message, char* scope){
    if(!condition){return;}
    int msg_size = 0;
    for (int i = 0;message[i]!=NULL; i++){
        msg_size += strlen(message[i]);
    }
    char* msg = malloc(sizeof(char)*(msg_size+1));
    int pos = 0;
    for (int i = 0;message[i]!=NULL; i++){
        int length = strlen(message[i]);
        strncpy(msg+pos,message[i],length);
        pos += length;

    }
    msg[msg_size] = '\0';
    if(!strcmp(scope,"ERROR")){printf("%sERROR:\t%s%s\n",RED,msg,RESET);exit(1);}
    size_t _ = 0; 
    char* file = ALL_SCOPES(scope,&_)[0];
    int line_size = 0, index = 0;
    char** lines =get_error_lines(file,line,3,&line_size,&index);
    int start_line = line - index;
    printf("%s==>%s%s\n",CYAN,file,RESET);
    for(int i=0;i<line_size;i++){
        printf("%s|%d\t%s%s%s\n",CYAN,start_line++,WHITE,lines[i],RESET);
        if(i==index){
            printf("%s|\t",RED);
            if(column-1 < strlen(msg)+2){
                char* columns = multiply_string(" ",column-1);
                printf("%s%s",columns,multiply_string("^",size));
                printf(" %s\n%s",msg,RESET);
            }
            else{
                char* columns =multiply_string(" ",column-strlen(msg)-2);
                printf("%s%s",columns,msg);
                printf(" %s\n%s",multiply_string("^",size),RESET);
            }
            
        }
    }
    exit(1);
}
