#include "include.h"

typedef struct{
    size_t size;
    Node** nodes;
}Replace;

static Node** add_node_to_array(Node** array,Node* value,int size,int index){
    Node** new_array = malloc(sizeof(Node*)*(size+1));
    for(int i = 0;i < size+1;i++){
        if(i < index){new_array[i] = array[i];}
        else if(i == index){new_array[i] = value;}
        else{new_array[i] = array[i-1];}
    }
    return new_array;
}

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
            replace_node->nodes = create_macro(replace_node->nodes,replace_node->size,scope,&replace_node->size);
            replace[id] = replace_node;
            index++;
            continue;
        }
        if(type == IDENTIFIER){
            int id = hash_id(nodes[index]->value);
            int line = nodes[index]->line;
            if(replace[id]!=NULL){
                Replace* replace_node = replace[id];
                for(int i=0;i<(int)replace_node->size;i++){

                    Node* node = replace_node->nodes[i];
                    nodes = add_node_to_array(nodes,node,size,index+1+i);
                    size++;
                }
                index++;
                ERROR_LOOP(1);

                continue;
            }
        }
        new_nodes[new_size++] = nodes[index++];
        new_nodes = realloc(new_nodes,sizeof(Node*)*(new_size+1));
    }
    *return_size = new_size;
    return new_nodes;
}