#include "include.h"

static int stack_size = 0;

static Bytecode** get_value(HashTable* table,Token** tokens,int index,size_t size,char* scope,size_t* return_size){
    Bytecode** bytecodes = malloc(sizeof(Bytecode*));
    size_t bytecode_size = 0;
    while(index<(int)size){
        Token* token = tokens[index++];
        bytecodes = realloc(bytecodes,sizeof(Bytecode*)*(bytecode_size+1));
        Bytecode* bytecode = malloc(sizeof(Bytecode));
        switch(token->type){
            case POP:{
                ERROR(stack_size-- == 0,token->line,(char*[]){"Stack is empty, cannot operate pop", NULL},__func__,scope);
                bytecode->type = POP;
                break;
            }   
            case INTEGER:{
                bytecode->type = INTEGER;
                bytecode->value = token->value;
                break;
            }
            case IDENTIFIER:{
                int id = hash_id(token->value);
                int found = hash_find(table,scope,id);
                ERROR(found == -1,token->line,(char*[]){"Identifier,\"",token->value,"\" not found", NULL},__func__,scope);
                Memory* memory = hash_get_memory(table,found,id);
                if(memory->type == INTEGER){
                    bytecode->type = VARIABLE;
                    bytecode->value = memory->value;
                }
                else{ERROR(1,token->line,(char*[]){"Bad read on identifier", NULL},__func__,scope);}
                break;
            }
            case EMPTY:{
                Token** hold = malloc(sizeof(Token*));
                size_t token_size = 0;
                for(;index<(int)size;index++){
                    hold = realloc(hold,sizeof(Token*)*(token_size+1));
                    hold[token_size++] = tokens[index];
                }
                size_t children_size = 0;
                Bytecode** children = build_bytecodes(table,hold,token_size,scope,&children_size); 
                bytecode->type = EMPTY;
                bytecode->children = children;
                bytecode->children_size = children_size;
                break;
            }
        }
        bytecodes[bytecode_size++] = bytecode;
    }
    *return_size = bytecode_size;
    return bytecodes;
}
static Bytecode* math_action(HashTable* table,Token* token,char* scope){
    int action = token->type;
    size_t size = 0;
    Bytecode** bytecodes = get_value(table,token->children,0,token->size,scope,&size); 
    ERROR(size < 2,token->line,(char*[]){"Bad read on, \"",TYPE(action),"\" action", NULL},__func__,scope); 
    Bytecode* bytecode = malloc(sizeof(Bytecode));
    bytecode->type = action;
    bytecode->children = bytecodes;
    bytecode->children_size = size;
    stack_size++;
    return bytecode;
}
static Bytecode* condition_action(HashTable* table,Token* token,char* scope){
    int action = token->type;
    size_t size = 0;
    Bytecode** bytecodes = get_value(table,token->children,0,token->size,scope,&size); 
    ERROR(size != 2,token->line,(char*[]){"Bad read on, \"",TYPE(action),"\" action", NULL},__func__,scope);
    Bytecode* bytecode = malloc(sizeof(Bytecode));
    bytecode->type = action;
    bytecode->children_size = size;
    bytecode->children = bytecodes;
    stack_size++;
    return bytecode;
}
static Bytecode* push(HashTable* table,Token* token,char* scope){
    size_t size = 0;
    Bytecode** bytecodes = get_value(table,token->children,0,token->size,scope,&size); 
    ERROR(size == 0,token->line,(char*[]){"Bad push, doesn't have a value", NULL},__func__,scope);
    Bytecode* bytecode = malloc(sizeof(Bytecode));
    bytecode->type = PUSH;
    bytecode->children_size = size;
    bytecode->children = bytecodes;
    stack_size += size;
    return bytecode;
}
static Bytecode* put(HashTable* table,Token* token,char* scope){
    size_t size = 0;
    Bytecode** bytecodes = get_value(table,token->children,0,token->size,scope,&size); 
    ERROR(size != 1,token->line,(char*[]){"Bad put, doesn't have a value", NULL},__func__,scope);
    Bytecode* bytecode = malloc(sizeof(Bytecode));
    bytecode->type = PUT;
    bytecode->children_size = size;
    bytecode->children = bytecodes;
    return bytecode;
}
static Bytecode* if_statement(HashTable* table,Token* token,char* scope){
    ERROR(token->size < 1,token->line,(char*[]){"Bad read on if statement", NULL},__func__,scope);
    Bytecode* bytecode = malloc(sizeof(Bytecode));
    size_t size = 0;
    Bytecode** bytecodes = get_value(table,token->children,0,token->size,scope,&size);
    ERROR(size != 2,token->line,(char*[]){"Bad read on if statement", NULL},__func__,scope);
    bytecode->type = token->type;   
    bytecode->values = realloc(bytecode->values,sizeof(Bytecode*)*(bytecode->value_size+1));
    bytecode->values[bytecode->value_size++] = bytecodes[0];
    bytecode->children = bytecodes[1]->children;
    bytecode->children_size = bytecodes[1]->children_size;
    return bytecode;
}
static Bytecode* variable(HashTable* table,Token* token,char* scope){
    ERROR(token->size < 2,token->line,(char*[]){"Bad read on variable declaration", NULL},__func__,scope);
    ERROR(token->children[0]->type != IDENTIFIER,token->line,(char*[]){"Bad read on variable name",NULL},__func__,scope);
    char* name = token->children[0]->value;
    int id = hash_id(name);
    int found = hash_find(table,scope,id);
    size_t size = 0;
    Bytecode** bytcodes= get_value(table,token->children,1,token->size,scope,&size);
    ERROR(size != 1,token->line,(char*[]){"Bad read on variable value",NULL},__func__,scope);
    Bytecode* bytecode = malloc(sizeof(Bytecode));
    bytecode->type = VARIABLE;
    bytecode->children_size = size;
    bytecode->children = bytcodes;
    if(found == -1){
        int* value = malloc(sizeof(int));
        int scope_id = hash_id(scope);
        *value = 0;
        hash_create_entry(table,scope_id,id, hash_create_memory(value,INTEGER));     
    }
    return bytecode;
}
Bytecode** build_bytecodes(HashTable* table,Token** tokens,size_t size,char* scope,size_t* return_size){
    Bytecode** bytecodes = malloc(sizeof(Bytecode*));
    size_t bytecode_size = 0;
    for(int i=0;i<(int)size;i++){
        Token* token = tokens[i];
        bytecodes = realloc(bytecodes,sizeof(Bytecode*)*(bytecode_size+1));
        switch(token->type){
            case PUSH:{
                bytecodes[bytecode_size++] = push(table,token,scope);
                break;
            } 
            case VARIABLE:{
                bytecodes[bytecode_size++] = variable(table,token,scope);
                break;
            }
            case PLUS:case MINUS:case MULTIPLY:case DIVIDE:{
                bytecodes[bytecode_size++] = math_action(table,token,scope);
                break;
            }
            case EQUAL_EQUAL:case BANG_EQUAL:case GT:case LT:{
                bytecodes[bytecode_size++] = condition_action(table,token,scope);
                break;
            }
            case PUT:{
                bytecodes[bytecode_size++] = put(table,token,scope);
                break;
            }
            case IF:case ELIF:{
                bytecodes[bytecode_size++] = if_statement(table,token,scope);
                break;
            }
        }        
    }
    *return_size = bytecode_size;
    return bytecodes;
}
