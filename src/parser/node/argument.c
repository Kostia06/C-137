#include "../private.h"

// Fully develope the argument
void parser_special_argument(Parser* parser){
    Node* node = parser->current_node;
    Node* child;
    node->type = ARGUMENT;
    node->children = vector_init();
    // gets every node inside the argument including the end of the argument, not the start of it
    int count = 1;
    for(;parser->index<(int)parser->nodes->size;parser->index++){
        child = vector_get(parser->nodes,parser->index);
        if(child->type == NEW_LINE){
            FREE_NODE(parser->memory,child); 
            continue;
        }
        vector_add(node->children,child);
        if(child->type == ARGUMENT_START){count++;}
        else if(child->type == ARGUMENT_END){count--;}
        if(count == 0){break;}
    }
    node->size = child->index - node->index + child->size;
    // handles the closing bracket
    if(count != 0){
        char* message = SYNC((char*[]){"Expected a closing bracket, got",NULL});
        error_single_init(parser->error,SYNTAX_ERROR,node->index,node->index+node->size,message);
        parser_error_skip(parser);
        return;
    }
    // checks if the argument is empty
    if(node->children->size == 1){
        child = vector_pop(node->children);
        // clean up
        FREE_NODE(parser->memory,child);
    }
}
// handle the parameters in the argument
void parser_function_argument(Parser* parser){
    Node* node = parser->current_node;
    node->children = new_parser(parser->error,parser->memory,node->children,EMPTY,parser->scope);
    for(int i=0;i<(int)node->children->size;i++){
        Node* parameter = vector_get(node->children,i);
        if(parameter->type != DECLARATION){
            char* message = SYNC((char*[]){"Expected a parameter, got a ",PRINT_TYPE(parameter->type),NULL}); 
            error_single_init(parser->error,SYNTAX_ERROR,parameter->index,parameter->index+parameter->size,message);
            parser_error_skip(parser);
            return;
        }
    }
    parser_add_node_to_cmd(parser);
}
// handle the argument in the expreesion
void parser_expression_argument(Parser* parser){
    Node* node = parser->current_node; 
    node->children = new_parser(parser->error,parser->memory,node->children,EXPRESSION,parser->scope);
    if(node->children->size != 1){
        char* message = SYNC((char*[]){"Expected one expression, got",NULL});
        error_single_init(parser->error,SYNTAX_ERROR,node->index,node->index+node->size,message);
        parser_error_skip(parser);
        return;
    }
    parser->current_node = vector_pop(node->children);
    FREE_NODE(parser->memory,node);
    node = vector_pop(parser->current_node->children);
    FREE_NODE(parser->memory,parser->current_node);
    parser->current_node = node;
    parser_add_node_to_cmd(parser);
}
