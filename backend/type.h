#ifndef BACKEND_TYPE_H
#define BACKEND_TYPE_H

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>

#include "../utils/include.h"
#include "../hash/include.h"

static LLVMTypeRef i1,i8,i16,i32,i64,i128,f16,f32,f64,f128;

typedef struct{
    AST* type,*arg;
}Function;
typedef struct{
    LLVMModuleRef module;
    LLVMContextRef context;
    Command* command;
    LLVMBuilderRef builder;
    HashTable* table;
    Function* function;
    char* current_scope;
}Backend;
typedef void (*backend_function)(Backend* backend);

// LLVM -------------------------------------------------------------------------
void llvm_init();
LLVMTypeRef llvm_create_type(AST* ast,char* scope);
// Inside -----------------------------------------------------------------------
void backend_function_declaration(Backend* backend);
void backend_return(Backend* backend);
// Outside ----------------------------------------------------------------------
void backend_error(Backend* backend);
// Inside -----------------------------------------------------------------------
void backend_loop_inside(Backend* backend, char* name);
// Value ------------------------------------------------------------------------
LLVMValueRef backend_value(Backend* backend,AST* ast);

static backend_function outside_states[P_END] = {
   [P_FUNCTION_DECLARATION] = backend_function_declaration, 
};
static backend_function inside_states[P_END] = {
    [P_RETURN] = backend_return,
};

#endif

