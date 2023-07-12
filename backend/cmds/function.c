#include "../private.h"

static LLVMTypeRef* function_para(LLVMContextRef context,AST* ast){
    LLVMTypeRef* paras = malloc(sizeof(LLVMTypeRef)*ast->children->size);
    for(int i = 0; i < ast->children->size; i++){
        AST* child = vector_get(ast->children,i);
        AST* type = vector_get(child->children,1);
        paras[i] = backend_create_type(context,type);
    }    
    return paras;
}


void backend_cmd_function(Controller* controller){
    PRINT_AST(controller->ast,0);
    int is_exported = controller->ast->actual_type == PUB;
    int has_return = controller->ast->children->size == 4;
    AST* ast = controller->ast;
    AST* ast_name, *ast_para,*ast_type, *ast_body;
    if(!has_return){
        ast_body = vector_pop(ast->children);
        ast_para = vector_pop(ast->children);
        ast_name = vector_pop(ast->children);
    }
    else{
        ast_body = vector_pop(ast->children);
        ast_type = vector_pop(ast->children);
        ast_para = vector_pop(ast->children);
        ast_name = vector_pop(ast->children);
    }
    LLVMTypeRef* paras = function_para(controller->context,ast_para);
    LLVMTypeRef type = has_return ? backend_create_type(controller->context,ast_type) : LLVMVoidType();
    LLVMTypeRef function_type = LLVMFunctionType(type,paras,ast_para->children->size,0);

    LLVMValueRef function = LLVMAddFunction(controller->module,ast_name->value.string,function_type);
    if(is_exported){LLVMSetLinkage(function,LLVMExternalLinkage);}
    LLVMBasicBlockRef entry = LLVMAppendBasicBlockInContext(controller->context,function,"entry");
    controller->builder = LLVMCreateBuilderInContext(controller->context);
    LLVMPositionBuilderAtEnd(controller->builder,entry);

    char* scope = controller->scope;
    controller->scope = SYNC((char*[]){scope,"!~!",ast_name->value.string,NULL});
    controller->ast = ast_body;
    backend_create_body(controller);

    LLVMDisposeBuilder(controller->builder);
}
