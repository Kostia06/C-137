#include "../private.h"
#include "../include.h"

// Argument for function
void parser_argument_function(Parser* parser){
    parser->ast->children = parse(parser->error,parser->token_memory,parser->memory,parser->ast->children,EMPTY,parser->scope);
    // Check if argument has correct ASTs 
    for(int i=0;i<parser->ast->children->size;i++){
        AST* ast = vector_get(parser->ast->children,i);
        if(ast->type != DECLARATION_ASSIGNMENT){
            char* message = SYNC((char*[]){"Function argument is not a DECLARATION ASSIGNMENT",NULL});
            error_init(parser->error,ERROR,ast->line,ast->column,ast->size,message);
            parser_error_skip(parser);
            return;
        }
    }
    parser->cmd->type = FUNCTION;
    if(parser->cmd->actual_type == EMPTY){
        parser->cmd->actual_type = FUNCTION;
    }
    parser_add_ast_to_cmd(parser);
}
// Argument for function calling
void parser_argument_function_call(Parser* parser){
    parser->ast->children = parse(parser->error,parser->token_memory,parser->memory,parser->ast->children,EXPRESSION,parser->scope);
    for(int i=0;i<parser->ast->children->size;i++){
        AST* ast = vector_get(parser->ast->children,i);
        if(ast->type != EXPRESSION){
            char* message = SYNC((char*[]){"Function argument is not a DECLARATION ASSIGNMENT",NULL});
            error_init(parser->error,ERROR,ast->line,ast->column,ast->size,message);
            parser_error_skip(parser);
            return;
        }
    }
    AST* id = vector_get(parser->cmd->children,parser->cmd->children->size-1);
    if(id->type != IDENTIFIER){
        char* message = SYNC((char*[]){"Function call is not a IDENTIFIER",NULL});
        error_init(parser->error,ERROR,id->line,id->column,id->size,message);
        parser_error_skip(parser);
        return;
    }
    vector_add(id->children,parser->ast);
    parser->ast = mem_init(parser->memory, sizeof(AST));
}
// Argument for scoped expression
void parser_argument_expression(Parser* parser){
    parser->ast->children = parse(parser->error,parser->token_memory,parser->memory,parser->ast->children,EXPRESSION,parser->scope);
    if(parser->ast->children->size != 1){
        char* message = SYNC((char*[]){"Expression scoped is not a single AST",NULL});
        error_init(parser->error,ERROR,parser->ast->line,parser->ast->column,parser->ast->size,message);
        parser_error_skip(parser);
        return;
    }
    parser->ast = vector_get(parser->ast->children,0);
    parser->ast = vector_get(parser->ast->children,0);
    parser_expression_add_ast(parser);
}
