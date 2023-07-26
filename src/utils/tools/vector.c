#include "../include.h"

Vector* vector_init(MemoryGroup* memory){
    Vector* v = mem_init(memory,sizeof(Vector));
    v->memory = memory;
    return v;
}
void vector_add(Vector* v, void* element){
    LinkedItem* item = malloc(sizeof(LinkedItem));
    item->element = element;
    if(v->size == 0){
        v->start = item;
        v->end = item;
    }
    else{
        v->end->next = item;
        item->prev = v->end;
        v->end = item;
    }
    v->size++;
}
void* vector_pop(Vector* v){
    if(v->size == 0){return NULL;}
    void* element = v->end->element;
    LinkedItem* prev = v->end->prev;
    free(v->end);
    v->end= prev;
    v->size--;
    return element;
}
void* vector_pop_by_index(Vector* v,int index){
    if(index >= v->size || index < 0){return NULL;}
    LinkedItem* item = v->start;
    for(int i=0;i<index;i++){item = item->next;}
    void* element = item->element;
    v->size--;
    if(index == 0){
        LinkedItem* next = item->next;
        v->start = next;
    }
    else if(index == v->size){
        LinkedItem* prev = item->prev;
        v->end = prev;
    }
    else{
        item->prev->next = item->next;
        item->next->prev = item->prev;
    }
    free(item);
    return element;

}
void* vector_get(Vector* v, size_t index){
    if(index >= v->size){return NULL;}
    LinkedItem* item = v->start;
    for(int i=0;i<index;i++){item = item->next;}
    return item->element;
}
void vector_remove(Vector* v,size_t index){
    if(index >= v->size || index < 0){return;}
    LinkedItem* item = v->start;
    for(int i=0;i<index;i++){item = item->next;}
    v->size--;
    if(index == 0){
        LinkedItem* next = item->next;
        v->start = next;
    }
    else if(index == v->size){
        LinkedItem* prev = item->prev;
        v->end = prev;
    }
    else{
        item->prev->next = item->next;
        item->next->prev = item->prev;
    }
    mem_free(v->memory,item->element);
    free(item);
}
void vector_clear(Vector* v){
    LinkedItem* item = v->start;
    for(int i=0;i<v->size;i++){
        LinkedItem* next = item->next;
        free(item->element);
        free(item);
        item = next;
    }
}
void vector_free(Vector* v){
    vector_clear(v);
    mem_free(v->memory,v);
}
