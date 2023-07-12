#include "../private.h"


void analyser_function(Analyser* analyser){
    // get the name of the function
    Node* name_ast = vector_get(analyser->ast->children,0);
    char* name = mem_init(analyser->memory,1); 
    strcat(name,name_ast->value.string);
    // get the parameters of the function
    Node* param_ast = vector_get(analyser->ast->children,1);
    Vector* params = vector_init();
    int param_with_value = 0;
    for(size_t i = 0; i < param_ast->children->size; i++){
        Node* param = vector_get(param_ast->children,i);
        if(param->children->size == 3){param_with_value = 1;}
        else if(param->children->size == 2 && param_with_value == 1){
            char* message = SYNC((char*[]){"Parameters without values must be first",NULL});
            error_single_init(analyser->error,SYNTAX_ERROR,param->index,param->index+param->size,message);
            return;
        }
        // param name
        Node* param_name_ast = vector_get(param->children,0);
        char* param_name = mem_init(analyser->memory,1);
        strcat(param_name,param_name_ast->value.string);
        // param type
        Node* param_type_ast = vector_get(param->children,1);
        Type* type = analyser_type(analyser,param_type_ast);
        // param itself
        FunctionParameter* func_param = mem_init(analyser->memory,sizeof(FunctionParameter));
        func_param->name = param_name;
        func_param->type = type;
        vector_add(params,func_param);
    }

    // clean up
    FREE_NODE(analyser->memory,name_ast);  
    FREE_NODE(analyser->memory,param_ast);
}
