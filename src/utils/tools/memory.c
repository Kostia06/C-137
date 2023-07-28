#include "../include.h"


MemoryGroup* mem_group_init(){
    MemoryGroup* memory = malloc(sizeof(MemoryGroup));
    memory->size = 0;
    return memory;
}
// adds a link
void* mem_init(MemoryGroup* memory,size_t size){
    void* ptr = malloc(size);
    LinkedItem* item = malloc(sizeof(LinkedItem));
    item->element = ptr;
    if(memory->size == 0){
        memory->start = item;
        memory->end = item;
    }
    else{
        memory->end->next = item;
        item->prev = memory->end;
        memory->end = item;
    }
    memory->size++;
    return ptr; 
}
// frees memory and connects the links
void mem_free(MemoryGroup* memory,void* ptr){
    if(!ptr){return;}
    LinkedItem* item = memory->start;
    int i = 0,worked = 0;
    for(;i<memory->size;i++){
        if(item->element == ptr){worked=1;break;}
        item = item->next;
    }
    if(!worked){return;}
    memory->size--;
    if(i == 0){
        LinkedItem* next = item->next;
        memory->start = next;
    }
    else if(i == memory->size){
        LinkedItem* prev = item->prev;
        memory->end = prev;
    }
    else{
        item->prev->next = item->next;
        item->next->prev = item->prev;
    }
    free(ptr);
    free(item);
}
void mem_group_free(MemoryGroup* memory){
    LinkedItem* item = memory->start;
    for(int i=0;i<memory->size;i++){
        LinkedItem* next = item->next;
        free(item->element); free(item);
        item = next;
    }
    free(memory);
}
void mem_print(MemoryGroup* memory){
    printf("Memory size: %zu\n",memory->size);
}
void* mem_copy(MemoryGroup* memory,void* ptr,size_t size){
    void* new_ptr = mem_init(memory,size);
    memcpy(new_ptr,ptr,size);
    return new_ptr;
}
