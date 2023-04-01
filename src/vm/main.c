#include "include.h"

void vm(TokenBlock** blocks, size_t size){
    for(int i=0;i<size;i++){
        TokenBlock* block = blocks[i];
        PRINT_BLOCK(block,0);
    }
}




