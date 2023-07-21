#include "../include.h"

#define INITIAL_CAPACITY 1

Vector* vector_init(){
    Vector* v = malloc(sizeof(Vector));
    v->data = malloc(INITIAL_CAPACITY * sizeof(void*));
    v->capacity = INITIAL_CAPACITY;
    v->size = 0;
    return v;
}
void vector_add(Vector* v, void* element){
    if (v->size == v->capacity){
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * sizeof(void*));
    }
    v->data[v->size] = element;
    v->size++;
}
void* vector_pop(Vector* v){
    if(v->size == 0){return NULL;}
    void* element = v->data[v->size - 1];
    if(--v->size < v->capacity / 4){
        v->capacity /= 2;
        v->data = realloc(v->data, v->capacity * sizeof(void*));
    }
    return element;
}
void* vector_pop_by_index(Vector* v,int index){
    if(index >= v->size){return NULL;}
    void* element = v->data[index];
    memmove(&v->data[index], &v->data[index+1], (v->size - index) * sizeof(void*));
    if(--v->size < v->capacity / 4){
        v->capacity /= 2;
        v->data = realloc(v->data, v->capacity * sizeof(void*));
    }
    return element;
}
void* vector_get(Vector* v, size_t index){
    if(index >= v->size){return NULL;}
    return v->data[index];
}
void vector_replace(Vector* v,size_t index,void* element){
    if(index >= v->size){return;}
    v->data[index] = element;
}
void vector_insert(Vector* v, size_t index,void* element){
    if(index > v->size){return;}
    if(v->size == v->capacity){
        v->capacity *= 2;
        v->data = realloc(v->data, v->capacity * sizeof(void*));
    }
    memmove(&v->data[index+1], &v->data[index], (v->size - index) * sizeof(void*));
    v->data[index] = element;
    v->size++;
}
void vector_remove(Vector* v,size_t index){
    if(index >= v->size){return;}
    free(v->data[index]);
    memmove(&v->data[index], &v->data[index+1], (v->size - index) * sizeof(void*));
    if(--v->size < v->capacity / 4){
        v->capacity /= 2;
        v->data = realloc(v->data, v->capacity * sizeof(void*));
    }
}
void vector_set_size(Vector* v,size_t size){
    if(size > v->size){
        if(size > v->capacity){
            v->capacity = size;
            v->data = realloc(v->data, v->capacity * sizeof(void*));
        }
        memset(&v->data[v->size], 0, (size - v->size) * sizeof(void*));
    }
    v->size = size;
}
void vector_clear(Vector* v){
    for(size_t i = 0; i < v->size; i++){
        void* element = v->data[i];
        if(element != NULL){free(element);}
    }
    v->size = 0;
    v->capacity = INITIAL_CAPACITY;
    v->data = realloc(v->data, INITIAL_CAPACITY * sizeof(void*));
}
void vector_free(Vector* v){
    vector_clear(v);
    free(v->data);
    free(v);
}
