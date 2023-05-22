#include "include.h"
#include "type.h"

#define DEBUG_MODULE    1

static void backend_set_states(backend_function* states, backend_function function){
    for(int i=0;i<P_END;i++){if(states[i] == NULL){states[i] = function;}}
}

void build_backend(Command** commands, size_t size, char* scope){
    llvm_init();
    Backend* backend = malloc(sizeof(Backend));
    backend->module = LLVMModuleCreateWithName("main");
    backend->context = LLVMGetGlobalContext();
    backend->table = malloc(sizeof(HashTable));
    backend->builder = LLVMCreateBuilderInContext(backend->context);
    backend_set_states(outside_states,backend_error);
    backend_set_states(inside_states,backend_error);
    for(int i=0;i<(int)size;i++){
        backend->command = commands[i];
        backend->current_scope = malloc(strlen(scope)+1);
        strcpy(backend->current_scope,scope);
        backend->function = NULL;
        backend_function function = outside_states[backend->command->type];
        function(backend);
    }
    #if DEBUG_MODULE == 1
        LLVMDumpModule(backend->module);
    #endif
    LLVMDisposeModule(backend->module);
}
