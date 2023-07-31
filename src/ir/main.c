#include "include.h"
#include "private.h"


void new_ir(Compiler* compiler,Vector* asts){
    for(int i=0;i<asts->size;i++){
        Node* ast = vector_get(asts,i);
        switch(ast->type){
            case STRUCT:{
                
                break;
            }
            case FUNCTION:{
                break;
            }
        }
    }            

}
