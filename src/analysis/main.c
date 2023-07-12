#include "include.h"
#include "private.h" 

Vector* new_analyzer(ErrorGroup* error,MemoryGroup* memory,Vector* asts,char* file){
    Analyser* analyser = mem_init(memory,sizeof(Analyser));
    analyser->memory = memory;
    analyser->error = error;
    analyser->scopes = vector_init();
    vector_add(analyser->scopes,file);
    // loop through the asts
    for(int i=0;i<asts->size;i++){
        analyser->ast = vector_get(asts,i);
        switch(analyser->ast->type){
            case FUNCTION:{
                analyser_function(analyser);
                break;
            }
        }
    }


    return NULL;
}
