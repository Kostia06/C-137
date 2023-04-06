#ifndef STACK_H
#define STACK_H

#include "../utils/include.h"

Stack* new_stack();
void stack_push(Stack* stack, int value);    
int stack_pop(Stack* stack,int line,char* scope);



#endif


