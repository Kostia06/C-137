#include "include.h"

typedef struct{
    size_t size;
    Node** nodes;
}Replace;


Node** create_macro(Node** nodes,size_t size,char* scope,size_t* return_size){
    Node** new_nodes = malloc(sizeof(Node*));
    size_t new_size = 0;    
    Replace* replace[MAX_HASH_SIZE];
    int index = 0;
    while(index<(int)size){ 
        int type = nodes[index]->type;
        if(type == MACRO_REPLACE){
            ERROR(index+1 >= (int)size || nodes[++index]->type != IDENTIFIER,0,(char*[]){"Macro replace error",NULL},__func__,NULL);
            int id = hash_id(nodes[index]->value);
            Replace* replace_node = malloc(sizeof(Replace));
            replace_node->size = 0;
            replace_node->nodes = malloc(sizeof(Node*));
            index++;
            while(index<(int)size && nodes[index]->type != NEW_LINE && nodes[index]->type != SEMICOLON){
                replace_node->nodes = realloc(replace_node->nodes,sizeof(Node*)*(replace_node->size+1));
                replace_node->nodes[replace_node->size++] = nodes[index++];
            }
            replace[id] = replace_node;
            continue;
        }
        if(type == IDENTIFIER){
            int id = hash_id(nodes[index]->value);
            int line = nodes[index]->line;
            if(replace[id]!=NULL){
                Replace* replace_node = replace[id];
                for(int i=0;i<(int)replace_node->size;i++){
                    Node* new_node = malloc(sizeof(Node));
                    memcpy(new_node,replace_node->nodes[i],sizeof(Node));
                    new_nodes[new_size++] = new_node;
                    new_nodes = realloc(new_nodes,sizeof(Node*)*(new_size+1));
                }
                index++;
                continue;
            }
        }
        new_nodes[new_size++] = nodes[index++];
        new_nodes = realloc(new_nodes,sizeof(Node*)*(new_size+1));
    }
    *return_size = new_size;
    return new_nodes;
}