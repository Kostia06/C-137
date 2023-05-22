#include "../type.h"

void backend_return(Backend* backend){
    ERROR(backend->command->ast_size != 1,backend->command->line,(char*[]){"Invalid return",NULL},backend->current_scope);
    AST* ast = backend->command->asts[0];
    PRINT_AST(ast,0);
    LLVMValueRef value = backend_value(backend,ast);
    printf("%s\n",LLVMPrintValueToString(value)); 
}

