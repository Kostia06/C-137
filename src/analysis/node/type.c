#include "../private.h"

Type* analyser_type(Analyser* analyser,Node* node){
    Type* type = mem_init(analyser->memory,sizeof(Type));
    // get the data type
    Node* data = vector_get(node->children,0);
    type->type = data->type;
    for(int i =0;i<data->children->size;i++){
        Node* trait = vector_get(data->children,i);
        switch(trait->type){
            case EXCLAMATION:
                type->is_mutable = 1;
                break;
        }
    }
    return type;
}
