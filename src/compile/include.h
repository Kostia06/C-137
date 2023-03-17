#ifndef COMPILE_H
#define COMPILE_H

#include "../utils/include.h"
#include "../hash/include.h"

#if  __has_include("llvm-c/Core.h")
    #include <llvm-c/Core.h>
    #include <llvm-c/ExecutionEngine.h>
    #include <llvm-c/Target.h>
    #include <llvm-c/Analysis.h>
    #include <llvm-c/Transforms/Scalar.h>
    #define LLVM_IS_INCLUDED     1
#else
    #define LLVM_IS_INCLUDED     0
#endif


void compile(HashTable* table,Config* config,NodeBlock** blocks,size_t size);

#endif
