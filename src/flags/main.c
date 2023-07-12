#include "include.h"

// Super simple cimpiler
CompilerFlags* flags_init(MemoryGroup* memory,char** args, int size){
    CompilerFlags* flags = mem_init(memory,sizeof(CompilerFlags));
    flags->files = vector_init();
    int i=0;
    while(i<size){
        if(!strcmp(args[i],"-f")){
            vector_add(flags->files,args[++i]);
        }
        i++;
    }

    return flags; 
}

