#ifndef FLAGS_H
#define FLAGS_H

#include "../utils/include.h" 

typedef struct{
    Vector* files;
}CompilerFlags;

CompilerFlags* flags_init(MemoryGroup* memory,char** args, int size);



#endif
