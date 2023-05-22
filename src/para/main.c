#include "include.h"


CompilerOptions* parameters(char** args, int size){
    CompilerOptions* options = malloc(sizeof(CompilerOptions));
    options->file = NULL;
    options->macro = 1;
    int i=0;
    while(i<size){
        if(args[i][0] == '-'){
            char* error[] = {"No value given after -f",NULL};
            ERROR(i+1>=size,0,0,0,error,"ERROR");
            char c = args[i++][1];
            switch(c){
                case 'f':{
                    options->file = malloc(sizeof(char)*(strlen(args[i])+1));
                    strcpy(options->file,args[i]);
                    break;             
                }
                case 'm':{
                    options->macro = 0;
                    i--;
                    break;
                }
            }
        }
        i++;
    }

    return options; 
}

