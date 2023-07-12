#ifndef ANALYSIS_PRIVATE_H
#define ANALYSIS_PRIVATE_H

#include "include.h"

typedef struct{
    MemoryGroup* memory;
    ErrorGroup* error;
    Vector* scopes;
    Node* ast;
}Analyser;

typedef struct{
    int type;
    int is_mutable;
}Type;

typedef struct{
    char* name;
    Type* type;
}FunctionParameter;


void analyser_function(Analyser* analyser);
Type* analyser_type(Analyser* analyser,Node* node);


#endif

