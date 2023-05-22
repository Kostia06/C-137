#include "../type.h"

void llvm_init(){
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    i1 = LLVMIntType(1);i8 = LLVMIntType(8);
    i16 = LLVMIntType(16);i32 = LLVMIntType(32);
    i64 = LLVMIntType(64);i128 = LLVMIntType(128);
    f16 = LLVMHalfType();f32 = LLVMFloatType();
    f64 = LLVMDoubleType();f128 = LLVMFP128Type();
}

LLVMTypeRef llvm_create_type(AST* ast,char* scope){
    LLVMContextRef context = LLVMGetGlobalContext();
    LLVMTypeRef type ;
    switch(ast->actual_type){
        case P_I1: type = LLVMInt1TypeInContext(context);break;
        case P_I8: type = LLVMInt8TypeInContext(context);break;
        case P_I16: type = LLVMInt16TypeInContext(context);break;
        case P_I32: type = LLVMInt32TypeInContext(context);break;
        case P_I64: type = LLVMInt64TypeInContext(context);break;
        case P_I128: type = LLVMInt128TypeInContext(context);break;
        case P_F16: type = LLVMHalfTypeInContext(context);break;
        case P_F32: type = LLVMFloatTypeInContext(context);break;
        case P_F64: type = LLVMDoubleTypeInContext(context);break;
        case P_F128: type = LLVMFP128TypeInContext(context);break;
        default: ERROR(1,ast->line,(char*[]){"Unknown type",NULL},scope);
    }
    for(int i=0;i<(int)ast->size;i++){
        AST* child = ast->children[i];
        switch(child->actual_type){
            case P_POINTER:{type = LLVMPointerType(type,0);break;}
            case P_ARRAY:{
                ERROR(child->size != 1 || child->children[0]->size != 1,child->line,(char*[]){"Array must have a size",NULL},scope); 
                ERROR(child->children[0]->children[0]->actual_type != P_INTEGER,child->line,(char*[]){"Array size must be an integer",NULL},scope);
                type = LLVMArrayType(type,(int)child->children[0]->children[0]->value.integer);
                break;
            }
        }
    }
    return type;
}
