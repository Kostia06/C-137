#include "include.h"


int vm(HashTable* table, Stack* stack, Token** tokens, size_t size, char* scope){
    /*
    for(int i =0;i<(int)size;i++){
        Token* token = tokens[i];   
        switch(token->type){
            case FUNCTION:{function_declaration(table,token,scope);break;}
            case VARIABLE:{variable_declaration(table,stack,token,scope);break;}
            case PLUS:case MINUS:
            case MULTIPLY:case DIVIDE:{
                get_action(table,stack,token,scope);
                break;
            }
            case EQUAL_EQUAL:case BANG_EQUAL:
            case GT:case LT:{
                get_condition(table,stack,token,scope);
                break;
            }
            case IF:case ELIF:{
                int result = statement_declaration(table,stack,token,scope);
                if(result != EMPTY){return result;}
                break;
            }
            case LOOP:{loop_declaration(table,stack,token,scope);break;}
            case PUSH:{push(table,stack,token,scope);break;}
            case PUT:{put(table,stack,token,scope);break;}
            case BREAK: case CONTINUE:{return token->type;}
        }
    }
    */
    return EMPTY;
}

