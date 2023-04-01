#include "include.h"


char* parameters(char** args, int size){
    char* path;
    int i=0;
    while(i<size){
        if(!strcmp("-f",args[i])){
            ERROR(++i>=size,0,(char*[]){"No path specified after -f",NULL},__func__,"PUBLIC");
            path = malloc(sizeof(char)*(strlen(args[i])+1));
            strcpy(path,args[i]);
        }
        i++;
    }
    return path;
}

