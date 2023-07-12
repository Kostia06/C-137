#ifndef BACKEND_PRIVATE_H
#define BACKEND_PRIVATE_H

#include <llvm-c/Core.h>

#include "../utils/include.h"
#include "../vector/include.h"
#include "../hash/include.h"

typedef struct{
    LLVMModuleRef module;
    LLVMBuilderRef builder;
    LLVMContextRef context;
    HashTable* table;
    AST* ast;
    Vector* asts;
    char* scope,* name;
} Controller;


void backend_cmd_function(Controller* controller);
void backend_cmd_module(Controller* controller);

LLVMTypeRef backend_create_type(LLVMContextRef context,AST* ast);
void backend_create_body(Controller* controller);
#endif
