#include "include.h"





char* compile_variable(HashTable* table,NodeBlock* block){
    ERROR(block->node_size < 2,block->line,(char*[]){"Variable declaration must have a name and a value",NULL},__func__,block->scope);
    ERROR(block->nodes[0]->type != IDENTIFIER,block->line,(char*[]){"Variable declaration must have a name",NULL},__func__,block->scope);
    char* name = (char*)block->nodes[0]->value;
    int name_id = hash_id(name);
    int found = hash_find(table,block->scope,name_id);  
    

    return "";
}

void compile(HashTable* table,Config* config,NodeBlock** blocks,size_t size){
    FILE* file = fopen(config->output_file,"w");
    for(int i=0;i<(int)size;i++){
        NodeBlock* block = blocks[i];
        PRINT_BLOCK(block,0);
    }
    fclose(file);
    if(!strcmp(config->compiler_mode,"run")){
        remove(config->output_file);
        return;
    }
    else if(!strcmp(config->compiler_mode,"compile")){
        remove(config->output_file);
        return;
    }
    else if(!strcmp(config->compiler_mode,"save")){
        return;
    }
    ERROR(1,0,(char*[]){"Compiler mode not found",NULL},__func__,"PUBLIC");
}