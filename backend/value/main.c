#include "../type.h"

typedef LLVMValueRef (*backend_value_action)(LLVMBuilderRef,LLVMValueRef,LLVMValueRef); 
typedef LLVMValueRef (*backend_value_extract)(Backend*,AST*);

static LLVMValueRef value_add(LLVMBuilderRef builder,LLVMValueRef left,LLVMValueRef right){return LLVMBuildAdd(builder,left,right,"add");}  
static LLVMValueRef value_sub(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildSub(builder,left,right,"sub");}
static LLVMValueRef value_mul(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildMul(builder,left,right,"mul");}
static LLVMValueRef value_div(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildSDiv(builder,left,right,"div");}
static LLVMValueRef value_mod(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildSRem(builder,left,right,"mod");}
static LLVMValueRef value_or(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildOr(builder,left,right,"or");}
static LLVMValueRef value_and(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildAnd(builder,left,right,"and");}
static LLVMValueRef value_equal_equal(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildICmp(builder,LLVMIntEQ,left,right,"equal_equal");}
static LLVMValueRef value_bang_equal(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildICmp(builder,LLVMIntNE,left,right,"bang_equal");}
static LLVMValueRef value_less(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildICmp(builder,LLVMIntSLT,left,right,"less");}
static LLVMValueRef value_less_equal(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildICmp(builder,LLVMIntSLE,left,right,"less_equal");}
static LLVMValueRef value_greater(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildICmp(builder,LLVMIntSGT,left,right,"greater");}
static LLVMValueRef value_greater_equal(LLVMBuilderRef builder,LLVMValueRef left, LLVMValueRef right){return LLVMBuildICmp(builder,LLVMIntSGE,left,right,"greater_equal");}

static LLVMValueRef value_integer(Backend* backend,AST* ast){return LLVMConstInt(LLVMInt32Type(),(int)ast->value.integer,0);}
static LLVMValueRef value_float(Backend* backend,AST* ast){return LLVMConstReal(LLVMFloatType(),ast->value.integer);}

static backend_value_action  actions[P_END] = {
    [P_ADD] = value_add, 
    [P_SUB] = value_sub,
    [P_MUL] = value_mul,
    [P_DIV] = value_div,
    [P_MOD] = value_mod,
    [P_OR] = value_or,
    [P_AND] = value_and,
    [P_EQUAL_EQUAL] = value_equal_equal,
    [P_BANG_EQUAL] = value_bang_equal,
    [P_LT] = value_less,
    [P_GT] = value_greater,
    [P_LT_EQUAL] = value_less_equal,
    [P_GT_EQUAL] = value_greater_equal,
}; 

static backend_value_extract values[P_END] = {
    [P_INTEGER] = value_integer,
    [P_FLOAT] = value_float,
    
    [P_ADD] = backend_value,
    [P_SUB] = backend_value,
    [P_MUL] = backend_value,
    [P_DIV] = backend_value,
    [P_MOD] = backend_value,
    [P_OR] = backend_value,
    [P_AND] = backend_value,
    [P_EQUAL_EQUAL] = backend_value,
    [P_BANG_EQUAL] = backend_value,
    [P_LT] = backend_value,
    [P_GT] = backend_value,
    [P_LT_EQUAL] = backend_value,
    [P_GT_EQUAL] = backend_value,
};

LLVMValueRef backend_value(Backend* backend,AST* ast){
    if(ast->type >= P_ADD && ast->type <= P_AND){
        ERROR(ast->size != 2,ast->line,(char*[]){"Invalid value",NULL},backend->current_scope);
        AST* left = ast->children[0];
        AST* right = ast->children[1];
        backend_value_extract extract_left = values[left->type];
        backend_value_extract extract_right = values[right->type];
        ERROR(extract_left == NULL || extract_right == NULL,ast->line,(char*[]){"Invalid value",NULL},backend->current_scope);
        LLVMValueRef left_value = extract_left(backend,left); 
        LLVMValueRef right_value = extract_right(backend,right);
        backend_value_action action = actions[ast->type];
        printf("%s\n",LLVMPrintValueToString(left_value)); 
        printf("%s\n",LLVMPrintValueToString(right_value)); 
        ERROR(action == NULL,ast->line,(char*[]){"Invalid value",NULL},backend->current_scope);
        return action(backend->builder,left_value,right_value);
    }
    ERROR(ast->size != 1,ast->line,(char*[]){"Invalid value",NULL},backend->current_scope);
    AST* child = ast->children[0];
    backend_value_extract extract = values[child->type];
    ERROR(extract == NULL,ast->line,(char*[]){"Invalid value",NULL},backend->current_scope);
    return extract(backend,child);
}
