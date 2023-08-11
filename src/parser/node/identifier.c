#include "../private.h"

static int get_identifier_type(Parser* parser,Node* node){
    int size = node->children->size;
    switch(size){
        case 0:{return VARIABLE;}
        case 1:{  
            Node* first = vector_get(node->children,0);
            if(first->type == EXPRESSION){return VARIABLE_WITH_VALUE;}
            if(first->type == ARGUMENT){return FUNCTION_CALL;}
            if(first->type == TYPE){return VARIABLE_WITH_TYPE;}
            break;
        }
        case 2:{
            Node* first = vector_get(node->children,0);
            Node* second = vector_get(node->children,1);
            if(first->type == TYPE && second->type == EXPRESSION){return VARIABLE_WITH_TYPE_AND_VALUE;}
            break;
        }
    }
    char* message = SYNC((char*[]){"Uknown IDENTIFIER type",NULL});
    error_single_init(parser->compiler->error,SYNTAX_ERROR,node->index,node->index+node->size,message);
    parser_error_skip(parser);
    return EMPTY;
}
// handles compound assignment operators
void parser_identifier_compound_op(Parser* parser){
    Node* next = vector_get(parser->nodes,parser->index);
    if(next->type != EQUAL){
        char* message = SYNC((char*[]){"Expected EQUAL after compound assignment operator",NULL});
        error_single_init(parser->compiler->error,SYNTAX_ERROR,next->index,next->index+next->size,message);
        parser_error_skip(parser);
        return;
    }
    Node* copy_identifier = parser_create_copy(parser,parser->cmd,parser->cmd->index,parser->cmd->size);
    Node* copy_op = parser_create_copy(parser,parser->current_node,parser->current_node->index,parser->current_node->size);
    vector_add_by_index(parser->nodes,copy_op,parser->index+1);
    vector_add_by_index(parser->nodes,copy_identifier,parser->index+1);
}
// end of the identifier
void parser_identifier_end(Parser* parser){
    parser->cmd->type = get_identifier_type(parser,parser->cmd);
    parser_end(parser);
}
// the end of the assignment
void parser_identifier_add(Parser* parser){
    parser->cmd->type = get_identifier_type(parser,parser->cmd);
    parser->cmd->size = parser->current_node->index - parser->cmd->index ;
    parser_add_cmd_to_cmds(parser);
    // clean up
    FREE_NODE(parser->compiler->memory,parser->current_node);
}
// get the value
void parser_identifier_value(Parser* parser){
    parser->hold_cmd = parser->cmd;
    parser->cmd = mem_init(parser->compiler->memory,sizeof(Node));
    if(!parser->cmd->children){
        parser->cmd->children = vector_init(parser->compiler->memory);
    }
    parser->cmd->index = parser->current_node->index;
    parser->cmd->type = EXPRESSION;
    // clean up
    FREE_NODE(parser->compiler->memory,parser->current_node);
}
