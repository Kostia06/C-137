#include "../private.h"

typedef LLVMTypeRef(*type_context)(LLVMContextRef context);
static type_context types[END] = {
    [I32] = LLVMInt32TypeInContext,
    [I64] = LLVMInt64TypeInContext,
    [F32] = LLVMFloatTypeInContext,
    [F64] = LLVMDoubleTypeInContext,
    [U32] = LLVMInt32TypeInContext,
    [U64] = LLVMInt64TypeInContext,
};


LLVMTypeRef backend_create_type(LLVMContextRef context,AST* ast){
    AST* ast_type = vector_pop(ast->children);
    LLVMTypeRef type = types[ast_type->type](context);

    PRINT_AST(ast_type,0);

    return type;
}
