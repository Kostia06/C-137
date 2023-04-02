#include "include.h"
typedef struct{
    size_t size;
    Token** tokens;
}Replace;



static Token* cp_token(Token* token){
    Token* new_token = malloc(sizeof(Token));
    new_token->type = token->type;
    new_token->value = token->value;
    new_token->line = token->line;
    return new_token;
}
static Token** add_token_to_array(Token** array,Token* value,int size,int index){
    Token** new_array = malloc(sizeof(Token*)*(size+1));
    for(int i = 0;i < size+1;i++){
        if(i < index){new_array[i] = array[i];}
        else if(i == index){new_array[i] = value;}
        else{new_array[i] = array[i-1];}
    }
    return new_array;
}

Token** create_macro(Token** tokens,size_t size,char* scope,size_t* return_size){
    Token** new_tokens = malloc(sizeof(Token*));
    size_t new_size = 0;
    Replace* replace[MAX_HASH_SIZE];
    int index = 0;
    while(index<(int)size){
        int type = tokens[index]->type;
        if(type == MACRO_REPLACE){
            ERROR(index+1 >= (int)size || tokens[++index]->type != IDENTIFIER,0,(char*[]){"Macro replace error",NULL},__func__,NULL);
            int id = hash_id(tokens[index]->value);
            Replace* replace_token = malloc(sizeof(Replace));
            replace_token->size = 0;
            replace_token->tokens = malloc(sizeof(Token*));
            index++;
            while(index<(int)size && tokens[index]->type != NEW_LINE && tokens[index]->type != SEMICOLON){
                replace_token->tokens = realloc(replace_token->tokens,sizeof(Token*)*(replace_token->size+1));
                replace_token->tokens[replace_token->size++] = tokens[index++];
            }
            replace_token->tokens = create_macro(replace_token->tokens,replace_token->size,scope,&replace_token->size);
            replace[id] = replace_token;
            index++;
            continue;
        }
        if(type == IDENTIFIER){
            int id = hash_id(tokens[index]->value);
            int line = tokens[index]->line;
            if(replace[id]!=NULL){
                Replace* replace_token = replace[id];
                for(int i=0;i<(int)replace_token->size;i++){
                    Token* token = cp_token(replace_token->tokens[i]);
                    token->line = line;
                    tokens = add_token_to_array(tokens,token,size,index+1+i);
                    size++;
                }
                index++;
                continue;
            }
        }
        new_tokens[new_size++] = tokens[index++];
        new_tokens = realloc(new_tokens,sizeof(Token*)*(new_size+1));
    }
    *return_size = new_size;
    return new_tokens;
}
