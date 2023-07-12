#include "../../private.h"
#include "private.h"

// The end of the expression in the argument
void parser_expression_end_argument(Parser* parser){
    AST* last_value = vector_pop(parser->cmd->children);
    vector_add(parser->hold_op->children,last_value);
    parser->hold_op = NULL;
    parser_add_cmd_to_cmds(parser);
}
// The end of the expression in the command
void parser_expression_end(Parser* parser){
    if(parser->hold_op == NULL){goto end;}
    AST* last_value = vector_pop(parser->cmd->children);
    vector_add(parser->hold_op->children,last_value);
    op_simple(parser,parser->hold_op); // check if the last op is simple
    op_complex(parser,parser->hold_op); // check if the last op is complex

    AST* last = vector_get(parser->cmd->children,0);
    op_simple(parser,last); // check if the last op is simple 
    op_complex(parser,last); // check if the last op is complex
    op_complex_identifier(parser,last); // check if the last op is complex with identifier
    op_cycle(parser,last); // check if there is a number deep in the tree
    op_cycle_identifier(parser,last); // check if there is a identifier deep in the tree
    parser->hold_op = NULL;
    goto end;
    end:
        if(parser->hold_cmd != NULL){
            vector_add(parser->hold_cmd->children,parser->cmd);
            parser->cmd = parser->hold_cmd;
            parser->hold_cmd = NULL;
        }
        parser_cmd_end(parser);
}
