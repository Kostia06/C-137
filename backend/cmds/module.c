#include "../private.h"

void backend_cmd_module(Controller* controller){
    if(controller->name != NULL){
        char* error[] = {"Module name is already defined",NULL};
        ERROR(1,controller->ast->line,controller->ast->column,-1,error,controller->scope);
    }
    AST* ast = vector_pop(controller->ast->children);
    controller->name = ast->value.string;
    controller->module = LLVMModuleCreateWithName(controller->name);
    controller->context = LLVMGetModuleContext(controller->module);
}
