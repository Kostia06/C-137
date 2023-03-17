#include "include.h"


static void split_type_and_value(Node** nodes,size_t size, int index,char* scope,Type** return_type,Node*** return_value,size_t* return_value_size){
    Type* type = malloc(sizeof(Type));
    Node** value = malloc(sizeof(Node*));
    size_t value_size = 0;
    Node* last, *current, *next;
    while(index<(int)size){
        current = nodes[index];
        if(index!=0){last=nodes[index-1];}
        if(index+1!=(int)size){next=nodes[index+1];}
        if(current->type >= TYPE_START && current->type <= TYPE_END){
            ERROR(type->type != EMPTY,current->line,(char*[]){"Type already declared",NULL},__func__,scope);
            type->type = current->type;
            while(next->type == STAR){
                type->pointer_size++;
                index++;
                current = next;
                if(index+1!=(int)size){next=nodes[index+1];}
            }
        }
        else{
            ERROR(type->type == EMPTY,current->line,(char*[]){"Type not declared",NULL},__func__,scope);
            value = realloc(value,sizeof(Node*) * (value_size + 1));
            value[value_size++] = current;
        }
        index++;
    }
    *return_type = type;
    *return_value = value;
    *return_value_size = value_size;
}

static void compile_variable(HashTable* table, NodeBlock* block){
    ERROR(block->node_size < 2,block->line,(char*[]){"Variable declaration must have a name and a value",NULL},__func__,block->scope);
    ERROR(block->nodes[0]->type != IDENTIFIER,block->line,(char*[]){"Variable declaration must have a name",NULL},__func__,block->scope);
    char* name = (char*)block->nodes[0]->value;
    int name_id = hash_id(name);
    int found = hash_find(table,block->scope,name_id);

    Type* type;
    Node** value;
    size_t value_size;  
    split_type_and_value(block->nodes,block->node_size,1,block->scope,&type,&value,&value_size);

    hash_entry_init(table,block->scope,name_id,type);
}
static void compile_function(HashTable* table, NodeBlock* block){
    ERROR(block->node_size < 2,block->line,(char*[]){"Function declaration must have a name, type and parameters",NULL},__func__,block->scope);
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
            // PRINT_BLOCK(block,0);
        }

        if(!strcmp(config->compiler_mode,"run")){

            return;
        }
        else if(!strcmp(config->compiler_mode,"compile")){

            return;
        }
        ERROR(1,0,(char*[]){"Compiler mode not found",NULL},__func__,"PUBLIC");
    #endif 
}