#include "../type.h"

void backend_function_declaration(Backend* backend){
    Command* command = backend->command;
    ERROR(command->ast_size != 3,command->line,(char*[]){"Invalid function declaration",NULL},backend->current_scope);
    ERROR(command->asts[0]->type != P_TYPE,command->line,(char*[]){"Invalid function declaration",NULL},backend->current_scope);
    ERROR(command->asts[1]->type != P_IDENTIFIER,command->line,(char*[]){"Invalid function declaration",NULL},backend->current_scope);
    ERROR(command->asts[2]->type != P_ARGUMENT,command->line,(char*[]){"Invalid function declaration",NULL},backend->current_scope);
    AST* type = command->asts[0];
    AST* arg = command->asts[2];
    char* name = command->asts[1]->value.string;
    int name_id = hash_id(name);
    int find = hash_find(backend->table,backend->current_scope,name_id);  
    ERROR(find != -1,command->line,(char*[]){"Function, \"",name,"\", already declared",NULL},backend->current_scope);
    Function* function = malloc(sizeof(Function));
    function->type = type;
    function->arg = arg;
    hash_add_value(backend->table,hash_id(backend->current_scope),name_id,function);
    backend->function = function;
    LLVMTypeRef return_type = llvm_create_type(type,backend->current_scope);
    LLVMTypeRef param_types[arg->size];
    for(int i=0;i<(int)arg->size;i++){
        AST* parameter = arg->children[i];
        param_types[i] = llvm_create_type(parameter->children[0],backend->current_scope);
    }
    LLVMTypeRef function_type = LLVMFunctionType(return_type,param_types,arg->size,0);
    LLVMValueRef function_value = LLVMAddFunction(backend->module,name,function_type);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function_value,"entry");
    //backend_loop_inside(backend,name);
}
