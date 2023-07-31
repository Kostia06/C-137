#include "../private.h"

// Capture the argument
void parser_special_argument(Parser* parser){
    parser_loop(parser,parser->current_node,ARGUMENT,ARGUMENT_START,ARGUMENT_END,0);
}
// handle the parameters in the argument
void parser_function_argument(Parser* parser){
    if(!parser->cmd->value.string){
        char* message = SYNC((char*[]){"Function does not have a name",NULL});
        error_single_init(parser->compiler->error,SYNTAX_ERROR,parser->cmd->index,parser->cmd->index+parser->cmd->size,message);
        return;        
    } 
    Node* node = parser->current_node;
    Vector* nodes = node->children;
    node->children = new_parser(parser->compiler,nodes,EMPTY);
    // clean up
    mem_free(parser->compiler->memory,nodes);
    for(int i=0;i<(int)node->children->size;i++){
        Node* parameter = vector_get(node->children,i);
        if(parameter->type != IDENTIFIER){
            char* message = SYNC((char*[]){"Expected a PARAMETER, got a ",PRINT_TYPE(parameter->type),NULL}); 
            error_single_init(parser->compiler->error,SYNTAX_ERROR,parameter->index,parameter->index+parameter->size,message);
            parser_error_skip(parser);
            return;
        }
    }
    parser_add_node_to_cmd(parser);
}
// handle the argument in the expreesion
void parser_expression_argument(Parser* parser){
    Node* node = parser->current_node; 
    Node* last = vector_get(parser->cmd->children,parser->cmd->children->size-1);
    Vector* nodes = node->children;
    node->children = new_parser(parser->compiler,nodes,EXPRESSION);
    // clean up
    mem_free(parser->compiler->memory,nodes);
    // checks if everything in the argument is an expression
    for(int i=0;i<(int)node->children->size;i++){
        Node* parameter = vector_get(node->children,i);
        if(parameter->type != EXPRESSION){
            char* message = SYNC((char*[]){"Expected a EXPRESSION, got a ",PRINT_TYPE(parameter->type),NULL}); 
            error_single_init(parser->compiler->error,SYNTAX_ERROR,parameter->index,parameter->index+parameter->size,message);
            parser_error_skip(parser);
            return;
        }
    }
    if(last && last->type == IDENTIFIER){
        if(!last->children){last->children = vector_init(parser->compiler->memory);}
        vector_add(last->children,node);
        return;
    }
    if(node->children->size != 1){
        char* message = SYNC((char*[]){"Expected one expression, got",NULL});
        error_single_init(parser->compiler->error,SYNTAX_ERROR,node->index,node->index+node->size,message);
        parser_error_skip(parser);
        return;
    }
    parser->current_node = vector_pop(node->children);
    FREE_NODE(parser->compiler->memory,node);

    node = vector_pop(parser->current_node->children);
    FREE_NODE(parser->compiler->memory,parser->current_node);

    parser->current_node = node;
    parser_add_node_to_cmd(parser);
}
// handle the argument in the block
void parser_declaration_argument(Parser* parser){
    Node* node = parser->current_node; 
    Node* last = vector_get(parser->cmd->children,parser->cmd->children->size-1);
    Vector* nodes = node->children;
    node->children = new_parser(parser->compiler,nodes,EXPRESSION);
    // clean up
    mem_free(parser->compiler->memory,nodes);
    // checks if everything in the argument is an expression
    for(int i=0;i<(int)node->children->size;i++){
        Node* parameter = vector_get(node->children,i);
        if(parameter->type != EXPRESSION){
            char* message = SYNC((char*[]){"Expected a EXPRESSION, got a ",PRINT_TYPE(parameter->type),NULL}); 
            error_single_init(parser->compiler->error,SYNTAX_ERROR,parameter->index,parameter->index+parameter->size,message);
            parser_error_skip(parser);
            return;
        }
    }
    if(parser->cmd->type == IDENTIFIER){
        parser_add_node_to_cmd(parser);
        return;
    }
    if(!last){
        char* message = SYNC((char*[]){"Expected a IDENTIFIER",NULL});
        error_single_init(parser->compiler->error,SYNTAX_ERROR,node->index,node->index+node->size,message);
        parser_error_skip(parser);
        return;
    } 
    if(last->type != IDENTIFIER){
        char* message = SYNC((char*[]){"Expected a IDENTIFIER, but got ",PRINT_TYPE(last->type),NULL});
        error_single_init(parser->compiler->error,SYNTAX_ERROR,last->index,last->index+last->size,message);
        parser_error_skip(parser);
        return;
    }
    if(!last->children){last->children = vector_init(parser->compiler->memory);} 
    vector_add(last->children,node);
}
