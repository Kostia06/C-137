#include "../../private.h"
#include "private.h"

// pretty much tries to find a number deep in the tree
void op_cycle(Parser* parser,AST* ast){
    return;
    AST* op = vector_get(ast->children,0);
    AST* value = vector_get(ast->children,1);
    if(value->type != INTEGER && value->type != FLOAT){return;} // must be a number
    
    AST* current = op;
    while(1){
        if(current->type != SUB && current->type != ADD){break;}
        AST* left = vector_get(current->children,0); 
        AST* right = vector_get(current->children,1);
        if(left->type == INTEGER || left->type == FLOAT){
            printf("progress\n");
        }
        else if(right->type == INTEGER || right->type == FLOAT){
            printf("progress\n");
        }
        if(left->type == SUB || left->type == ADD){
            current = left;
        }
        else if(right->type == SUB || right->type == ADD){
            current = right;
        }
        break;
    }


}
// pretty much tries to find a identifier deep in the tree
void op_cycle_identifier(Parser* parser,AST* ast){

}
