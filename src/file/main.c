#include "include.h"

char* read_file(char* file_name){
    struct stat sb;
    ERROR(stat(file_name, &sb) == -1,0, (char *[]){"Directory \"",file_name,"\" not found",NULL},__func__,"PUBLIC");
    ERROR(S_ISDIR(sb.st_mode)==1,0,(char*[]){"File not found",NULL},__func__,"PUBLIC");
    FILE* fp = fopen(file_name, "r");
    ERROR(fp == NULL,0, (char *[]){"Error opening file \"",file_name,"\"",NULL},__func__,"PUBLIC");
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    char* buffer = (char*) malloc(sizeof(char) * (file_size + 1));
    size_t result = fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';
    fclose(fp);
    return buffer;
}
char** get_error_lines(char* file_name,int index,int size,int* return_size,int* return_index){
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