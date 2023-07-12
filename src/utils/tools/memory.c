#include "../include.h"

MemoryGroup* mem_group_init(){
    MemoryGroup* memory = malloc(sizeof(MemoryGroup));
    memory->ptrs = vector_init();
    return memory;
}
void* mem_init(MemoryGroup* memory,size_t size){
    void* ptr = malloc(size);
    vector_add(memory->ptrs,ptr);
    return ptr; 
}
void mem_add(MemoryGroup* memory,void* ptr){
    vector_add(memory->ptrs,ptr);
}
void* mem_copy(MemoryGroup* memory,void* ptr){
    void* new_ptr = malloc(sizeof(void*));
    memcpy(new_ptr,ptr,sizeof(void*));
    vector_add(memory->ptrs,new_ptr);
    return new_ptr;
}
void mem_free(MemoryGroup* memory,void* ptr){
    for(int i = 0;i < memory->ptrs->size;i++){
        if(vector_get(memory->ptrs,i) == ptr){
            vector_remove(memory->ptrs,i);
            return;
        }
    }
}
void mem_group_free(MemoryGroup* memory){
    vector_free(memory->ptrs);
    free(memory);
}
void mem_print(MemoryGroup* memory){
    printf("Memory size: %zu\n",memory->ptrs->size);
}
