#include "include.h"

static Node* new_node(Token* token){
    Node* node = malloc(sizeof(Node));
    node->type = token->type;
    node->value = token->value;
    node->line = token->line;
    node->column = token->column;
    free(token);
    return node;
}
static Node* loop_nodes(Token** tokens,size_t size,int index,char* scope,int left,int right,int *return_index){
    Node* node = malloc(sizeof(Node));
    Token** hold = malloc(sizeof(Node*));
    size_t hold_size = 0;
    int count = 1;
    int line = tokens[index]->line;
    int column = tokens[index]->column;
    index++;
    for(;index<(int)size;index++){
        Token* current_token = tokens[index];
        if(current_token->type==left){count++;free(current_token);}
        else if(current_token->type==right){count--;free(current_token);}
        if(!count){break;}
        hold = realloc(hold,sizeof(Node*)*(hold_size+1));
        hold[hold_size++] = current_token;
    }
    ERROR(count != 0,line,(char*[]){"Unmatched \"",TYPE(right),"\"",NULL},__func__,scope);
    size_t hold_return_size = 0;
    Node** hold_nodes = build_nodes(hold,scope,hold_size,&hold_return_size);
    node->nodes = realloc(node->nodes,sizeof(Node*)*(hold_return_size));
    node->nodes = hold_nodes;
    node->node_size = hold_return_size;
    *return_index = index;
    return node;
}

Node** build_nodes(Token** tokens,char* scope,size_t size,size_t* return_size){
    Node** nodes = malloc(sizeof(Node*));
    size_t node_size = 0;
    int index =0;
    while(index<(int)size){
        Token* current_token = tokens[index];
        nodes = realloc(nodes,sizeof(Node*)*(node_size+1));
        switch(current_token->type){
            case LPAREN:{
                Node* node = loop_nodes(tokens,size,index,scope,LPAREN,RPAREN,&index);
                node->type = ARGUMENT;
                node->line = current_token->line;
                nodes[node_size++] = node;
                break;
            }
            case LBRACKET:{
                Node* node = loop_nodes(tokens,size,index,scope,LBRACKET,RBRACKET,&index);
                node->line = current_token->line;
                node->type = ARRAY;
                nodes[node_size++] = node;
                break;
            }
            default:{
                nodes[node_size++] = new_node(current_token);
                break;
            }
        }
        index++;
    }
    *return_size = node_size;
    return nodes;
}

