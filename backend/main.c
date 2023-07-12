#include "include.h"
#include "private.h"

void backend_compile(Vector* asts, char* scope){
    Controller* controller = malloc(sizeof(Controller));
    controller->table = malloc(sizeof(HashTable));
    controller->asts = asts;
    for(int i =0;i<(int)asts->size;i++){
        controller->ast = vector_get(asts,i);
        switch(controller->ast->actual_type){
            case FUNCTION: case PUB:{
                backend_cmd_function(controller);
                break;
            }
            case MODULE:{
                backend_cmd_module(controller);
                break;
            }

        }
    }
    LLVMDumpModule(controller->module);
    LLVMDisposeModule(controller->module);
}
