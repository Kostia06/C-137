#ifndef VECTOR_H
#define VECTOR_H

#include "../utils/include.h"


Vector* vector_init();
void vector_add(Vector* v, void* element);
void* vector_pop(Vector* v);
void vector_insert(Vector* v, size_t index,void* element);
Vector* vector_clone(const Vector* v);
void vector_clear(Vector* v);
void* vector_get(Vector* v, size_t index);
void vector_replace(Vector* v,size_t index,void* element);
void vector_remove(Vector* v,size_t index);

#endif

