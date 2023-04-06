#include "include.h"

Token* new_ast(){
    Token* token = malloc(sizeof(Token));
    token->value = malloc(sizeof(int));
    token->children = malloc(sizeof(Token*));
    token->size = 0;
    return token;
}

Token** level_ast(Token** tokens, size_t size, char* scope, size_t* return_size){
    Token** ast = malloc(sizeof(Token*));
    size_t ast_size = 0;
    int i=0;
    while(i<(int)size){
        Token* token = tokens[i++];
        switch(token->type){
            case FUNCTION:
            case IF:case ELIF:
            case LOOP:{
                Token** hold = malloc(sizeof(Token*));
                size_t hold_size = 0;
                int spacing = *(int*)token->value;
                ERROR(i>=(int)size,token->line,(char*[]){"No body",NULL},__func__,scope);                    
                if(tokens[i]->type == FUNCTION_START){
                    int count =1;i++;
                    for(;i<(int)size;i++){
                        if(tokens[i]->type == FUNCTION_START){count++;}
                        else if(tokens[i]->type == FUNCTION_END){count--;}
                        if(!count){break;}
                        hold = realloc(hold,sizeof(Token*)*(hold_size+1));
                        hold[hold_size++] = tokens[i];
                    }
                    i++;
                    ERROR(count > 0,token->line,(char*[]){"No end",NULL},__func__,scope);
                }
                else{
                    for(;i<(int)size;i++){
                        if(*(int*)tokens[i]->value <= spacing){break;}
                        hold = realloc(hold,sizeof(Token*)*(hold_size+1));
                        hold[hold_size++] = tokens[i];
                    }
                }
                ERROR(hold_size == 0,token->line,(char*[]){"No body",NULL},__func__,scope);
                hold = level_ast(hold,hold_size,scope,&hold_size);
                token->children = realloc(token->children,sizeof(Token*)*(token->size+1));
                token->children[token->size++] = malloc(sizeof(Token));
                for(int j=0;j<(int)hold_size;j++){
                    token->children = realloc(token->children,sizeof(Token*)*(token->size+1));
                    token->children[token->size++] = hold[j];
                }

            }
            default:{
                ERROR(token->type == FUNCTION_START,token->line,(char*[]){"Block was not recognized",NULL},__func__,scope);
                ast = realloc(ast,sizeof(Token*)*(ast_size+1));
                ast[ast_size++] = token;
                break;
            }
        }
    }
    *return_size = ast_size;
    return ast;
}
Token** ast(Token** tokens, size_t size, char* scope,size_t* return_size){
    Token* ast = malloc(sizeof(Token)), * hold = new_ast();
    int i = 0, type = EMPTY, next_spacing = 0;  
    while(i<size){
        Token* token = tokens[i++];
        if(token->type == NEW_LINE || token->type == SEMICOLON || token->type == FUNCTION_START){
            if(hold->size==0 && type == EMPTY){continue;}
            ERROR(type == EMPTY,token->line,(char*[]){"No parser type",NULL},__func__,scope);
            *(int*)hold->value = next_spacing;
            hold->type = type;
            hold->line = token->type == NEW_LINE ? token->line-1 : token->line;

            ast->children = realloc(ast->children,sizeof(Token*)*(ast->size+1));
            ast->children[ast->size++] = hold;

            hold = new_ast();
            type = EMPTY;
            next_spacing = token->type == FUNCTION_START ? *(int*)tokens[i]->value:*(int*)token->value;
            if(token->type != FUNCTION_START){continue;}
        }
        if(token->type == FUNCTION_START || token->type == FUNCTION_END){
            ast->children = realloc(ast->children,sizeof(Token*)*(ast->size+1));
            ast->children[ast->size++] = token;
            continue;
        }
        if(token->type >= ACTION_START && token->type <= ACTION_END){
            ERROR(type!=EMPTY,token->line,(char*[]){"Too many parser types",NULL},__func__,scope);    
            type = token->type;
            continue;
        }
        hold->children = realloc(hold->children,sizeof(Token*)*(hold->size+1)); 
        hold->children[hold->size++] = token;
    }
    ast->children = level_ast(ast->children,ast->size,scope,&ast->size);
    *return_size = ast->size;
    return ast->children;
}
