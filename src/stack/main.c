#include "include.h"

Stack* new_stack(){
    Stack* stack = malloc(sizeof(Stack));
    stack->top = -1;
    return stack;
}
void stack_push(Stack* stack, int value){stack->elements[++stack->top] = value;}    
int stack_pop(Stack* stack,int line,char* scope){
    ERROR(stack->top < 0,line,(char*[]){"Stack is empty",NULL},__func__,scope);
    return stack->elements[stack->top--];
}
