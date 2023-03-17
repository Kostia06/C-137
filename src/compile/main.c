#include "include.h"


void compile_variable(HashTable* table, NodeBlock* block){
    ERROR(block->node_size < 2,block->line,(char*[]){"Variable declaration must have a name and a value",NULL},__func__,block->scope);
    ERROR(block->nodes[0]->type != IDENTIFIER,block->line,(char*[]){"Variable declaration must have a name",NULL},__func__,block->scope);
    char* name = (char*)block->nodes[0]->value;
    int name_id = hash_id(name);
    int found = hash_find(table,block->scope,name_id);
    

}
void compile_function(HashTable* table, NodeBlock* block){
    ERROR(block->node_size < 2,block->line,(char*[]){"Function declaration must have a name and a value",NULL},__func__,block->scope);
    ERROR(block->nodes[0]->type != IDENTIFIER,block->line,(char*[]){"Function declaration must have a name",NULL},__func__,block->scope);
    char* name = (char*)block->nodes[0]->value;
    int name_id = hash_id(name);
    int scope_id = hash_find(table,block->scope,name_id);
    ERROR(scope_id != -1,block->line,(char*[]){"Function already declared",NULL},__func__,block->scope);



}

void compile(HashTable* table,Config* config,NodeBlock** blocks,size_t size){
    ERROR(!LLVM_IS_INCLUDED,0,(char*[]){"LLVM is not installed",NULL},__func__,"PUBLIC");
    #if LLVM_IS_INCLUDED == 1
        LLVMInitializeNativeTarget();
        LLVMInitializeNativeAsmPrinter();
        LLVMInitializeNativeAsmParser();

        LLVMModuleRef module = LLVMModuleCreateWithName("main");
        LLVMBuilderRef builder = LLVMCreateBuilder();
        
    
        for(int i=0;i<(int)size;i++){
            NodeBlock* block = blocks[i];
            switch(block->type){
                case VAR:{compile_variable(table,block);break;}
                case FN:{compile_function(table,block);break;}
                default:{ERROR(1,block->line,(char*[]){"Block type not found",NULL},__func__,block->scope);}    
            }
            PRINT_BLOCK(block,0);
        }

        if(!strcmp(config->compiler_mode,"run")){
            // remove(config->output_folder);
            return;
        }
        else if(!strcmp(config->compiler_mode,"compile")){
            // remove(config->output_folder);
            return;
        }
        else if(!strcmp(config->compiler_mode,"save")){
            return;
        }
        ERROR(1,0,(char*[]){"Compiler mode not found",NULL},__func__,"PUBLIC");
    #endif 
}