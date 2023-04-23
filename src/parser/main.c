#include "include.h"
#include "type.h"

static int spacing = 0;
static int last_end_type = T_NEW_LINE;

static void parser_advance(Parser* parser){
    parser->current = parser->tokens[parser->index++];
}
static AST* parser_get_last_ast(Parser* parser){return parser->command->asts[parser->command->ast_size-1];}
static void parser_set_empty(Parser* parser){parser->state = P_EMPTY;}

void parser_add_identifier(Parser* parser){
    AST* ast = malloc(sizeof(AST));
    ast->type = P_IDENTIFIER;
    ast->actual_type = P_IDENTIFIER;
    ast->line = parser->current->line;
    ast->value.string = parser->current->value.string;
    parser->command->asts = realloc(parser->command->asts,sizeof(AST*)*(parser->command->ast_size+1));
    parser->command->asts[parser->command->ast_size++] = ast;
    parser_advance(parser);
}
void parser_add_number(Parser* parser){
    AST* ast = malloc(sizeof(AST));
    int type = parser->current->type == T_INTEGER ? P_INTEGER : P_FLOAT;
    ast->type = type;
    ast->actual_type = type;
    ast->line = parser->current->line;
    ast->value.integer = parser->current->value.integer;
    parser->command->asts = realloc(parser->command->asts,sizeof(AST*)*(parser->command->ast_size+1));
    parser->command->asts[parser->command->ast_size++] = ast;
    parser_advance(parser);
}
void parser_add_action(Parser* parser){
    AST* ast = malloc(sizeof(AST));
    int type;
    switch(parser->current->type){
        case T_ADD: type = P_ADD; break;
        case T_SUB: type = P_SUB; break;
        case T_MUL: type = P_MUL; break;
        case T_DIV: type = P_DIV; break;
        default: ERROR(1,parser->current->line,(char*[]){"Unknown action",NULL},__func__,parser->scope);
    }
    ast->type = type;
    ast->actual_type = type;
    ast->line = parser->current->line;
    parser->command->asts = realloc(parser->command->asts,sizeof(AST*)*(parser->command->ast_size+1));
    parser->command->asts[parser->command->ast_size++] = ast;
    parser_advance(parser);
}
void parser_argument(Parser* parser){
    Token* token = parser->current;
    AST* argument = malloc(sizeof(AST));
    argument->type = P_ARGUMENT;
    argument->actual_type = T_ARGUMENT;
    argument->line = parser->current->line;
    argument->children = malloc(sizeof(AST*));
    for(int i=0;i<(int)token->size;i++){
        Token* parameter = token->value.children[i];
        int line = token->line;
        Parser* new_parser = malloc(sizeof(Parser));
        new_parser->tokens = parameter->value.children;
        new_parser->token_size = parameter->size;
        new_parser->state = P_EMPTY;
        new_parser->scope = parser->scope;
        new_parser->command = malloc(sizeof(Command));
        new_parser->command_size = 0;
        parser_advance(new_parser);
        while(new_parser->index <= new_parser->token_size){
            parser_function function = states[new_parser->state][new_parser->current->type];
            if(function!=NULL){function(new_parser);}
            else{parser_advance(new_parser);}
        }
        ERROR(new_parser->command_size != 0,line,(char*[]){"Invalid argument",NULL},__func__,parser->scope);
        for(int j=0;j<(int)new_parser->command->ast_size;j++){
            argument->children = realloc(argument->children,sizeof(AST*)*(argument->size+1));
            argument->children[argument->size++] = new_parser->command->asts[j];
        }
    }
    if(parser->command->ast_size > 0 && parser->command->asts[parser->command->ast_size-1]->type == P_IDENTIFIER){
        AST* identifier = parser->command->asts[parser->command->ast_size-1];
        identifier->children = realloc(identifier->children,sizeof(AST*)*(identifier->size+1));
        identifier->children[identifier->size++] = argument;
    }
    else{
        parser->command->asts = realloc(parser->command->asts,sizeof(AST*)*(parser->command->ast_size+1));
        parser->command->asts[parser->command->ast_size++] = argument;
    }
    parser_advance(parser);
}
// Type ------------------------------------------------------------------------------------------------
void parser_create_type(Parser* parser){
    AST* ast = malloc(sizeof(AST));
    parser->state = P_TYPE;
    ast->type = P_TYPE;
    ast->actual_type = parser->current->type;
    ast->line = parser->current->line;
    ast->children = malloc(sizeof(AST*));
    parser->command->asts = realloc(parser->command->asts,sizeof(AST*)*(parser->command->ast_size+1));
    parser->command->asts[parser->command->ast_size++] = ast;
    parser_advance(parser);
}
void parser_type_add_child(Parser* parser){
    ERROR(parser->command->ast_size == 0,parser->current->line,(char*[]){"Type must be in the beginning",NULL},__func__,parser->scope);
    AST* pointer = malloc(sizeof(AST));
    pointer->type = parser->current->type == T_MUL ? P_POINTER : P_ARRAY;
    pointer->actual_type = parser->current->type;
    pointer->line = parser->current->line;
    AST* ast = parser_get_last_ast(parser);
    ast->children = realloc(ast->children,sizeof(AST*)*(ast->size+1));
    ast->children[ast->size++] = pointer;
    parser_advance(parser);
}
void parser_too_many_type(Parser* parser){
    ERROR(1,parser->current->line,(char*[]){"Too many types",NULL},__func__,parser->scope);
}

// Command type ---------------------------------------------------------------------------------------
void parser_give_command_type(Parser* parser){
    ERROR(parser->command->type != P_EMPTY,parser->current->line,(char*[]){"Too many action types",NULL},__func__,parser->scope);
    ERROR(parser->command->ast_size != 0,parser->current->line,(char*[]){"Action type must be in the beginning",NULL},__func__,parser->scope);
    switch(parser->current->type){
        case T_IF:parser->command->type = P_IF;break;
        case T_ELIF:parser->command->type = P_ELIF;break;
        case T_LOOP:parser->command->type = P_LOOP;break;
        case T_BREAK:parser->command->type = P_BREAK;break;
        case T_CONTINUE:parser->command->type = P_CONTINUE;break;
        case T_RETURN:parser->command->type = P_RETURN;break;
    }
    parser_advance(parser);
}
// Declaration ----------------------------------------------------------------------------------------
void parser_parameters(Parser* parser){
    Token* token = parser->current;
    AST* argument = malloc(sizeof(AST));
    argument->type = P_ARGUMENT;
    argument->actual_type = T_ARGUMENT;
    argument->line = parser->current->line;
    argument->children = malloc(sizeof(AST*));
    for(int i=0;i<(int)token->size;i++){
        Token* parameter = token->value.children[i];
        Parser* new_parser = malloc(sizeof(Parser));
        new_parser->tokens = parameter->value.children;
        new_parser->token_size = parameter->size;
        new_parser->state = P_EMPTY;
        new_parser->scope = parser->scope;
        new_parser->command = malloc(sizeof(Command));
        parser_advance(new_parser);
        while(new_parser->index <= new_parser->token_size){
            parser_function function = states[new_parser->state][new_parser->current->type];
            if(function!=NULL){function(new_parser);}
            else{parser_advance(new_parser);}
        }
        ERROR(new_parser->command->ast_size != 2,parser->current->line,(char*[]){"Parameter must have a type and a name",NULL},__func__,parser->scope);
        AST* type = new_parser->command->asts[0];
        AST* name = new_parser->command->asts[1];
        ERROR(type->type != P_TYPE,parser->current->line,(char*[]){"Parameter must have this order <type> <identifier>",NULL},__func__,parser->scope);
        ERROR(name->type != P_IDENTIFIER,parser->current->line,(char*[]){"Parameter must have this order <type> <identifier>",NULL},__func__,parser->scope);
        AST* parameter_ast = malloc(sizeof(AST));
        parameter_ast->type = P_EMPTY;
        parameter_ast->actual_type = P_EMPTY;
        parameter_ast->line = parser->current->line;
        parameter_ast->children = malloc(sizeof(AST*)*2);
        parameter_ast->children[0] = type;
        parameter_ast->children[1] = name;
        parameter_ast->size = 2;
        argument->children = realloc(argument->children,sizeof(AST*)*(argument->size+1));
        argument->children[argument->size++] = parameter_ast;
    }
    parser->command->asts = realloc(parser->command->asts,sizeof(AST*)*(parser->command->ast_size+1));
    parser->command->asts[parser->command->ast_size++] = argument;
    parser_advance(parser);
}
void parser_declaration(Parser* parser){
    ERROR(parser->command->type != P_EMPTY,parser->current->line,(char*[]){"Declaration must have a type",NULL},__func__,parser->scope);
    parser_advance(parser); 
    if(parser->current->type == T_ARGUMENT){
        parser->command->type = P_FUNCTION_DECLARATION;
        parser_parameters(parser);
    }
    else{
        parser->command->type = P_VARIABLE_DECLARATION;
    }
}
// End of Command -------------------------------------------------------------------------------------
void parser_check_call(Parser* parser){
    if(parser->command->ast_size == 1 && parser->command->type == P_EMPTY){
        AST* name = parser->command->asts[0];
        if(name->type == P_IDENTIFIER && name->size == 1 && name->children[0]->type == P_ARGUMENT){
            parser->command->type = P_FUNCTION_CALL;
        }
    }
}

void parser_new_line(Parser* parser){
    parser_check_call(parser);
    ERROR(parser->command->type == P_EMPTY && parser->command->ast_size != 0,parser->current->line,(char*[]){"Command must have a type",NULL},__func__,parser->scope);
    int current_space = spacing;
    spacing = parser->current->value.integer;
    parser->command->line = parser->current->line;
    parser_advance(parser);
    if(parser->command->ast_size == 0 && parser->command->type == P_EMPTY){return;}

    parser->command->spacing = current_space;
    parser->command->declaration = last_end_type;

    parser->commands = realloc(parser->commands,sizeof(Command*)*(parser->command_size+1));
    parser->commands[parser->command_size++] = parser->command;
    parser->command = malloc(sizeof(Command));

    last_end_type = T_NEW_LINE;
}
void parser_semicolon(Parser* parser){
    parser_check_call(parser);
    ERROR(parser->command->type == P_EMPTY && parser->command->ast_size != 0,parser->current->line,(char*[]){"Command must have a type",NULL},__func__,parser->scope);
    parser->command->line = parser->current->line;
    parser_advance(parser);
    if(parser->command->ast_size == 0 && parser->command->type == P_EMPTY){return;}
    
    parser->command->spacing = spacing;
    parser->command->declaration = last_end_type;
    
    parser->commands = realloc(parser->commands,sizeof(Command*)*(parser->command_size+1));
    parser->commands[parser->command_size++] = parser->command;
    parser->command = malloc(sizeof(Command));

    last_end_type = T_SEMICOLON;
}

void parser_set_states(int state,parser_function function){
    for(int i=0;i<T_END;i++){
        if(states[state][i] == NULL){states[state][i] = function;}
    }
}
Command** parser_block(Command** commands,size_t size,char* scope,size_t* return_size){
    Command** blocks = malloc(sizeof(Command*));
    size_t block_size = 0;
    int index=0;
    while(index<(int)size){
        Command* command = commands[index++];
        switch(command->type){
            case P_FUNCTION_DECLARATION:
            case P_IF: case P_ELIF:
            case P_LOOP:{
                Command** hold = malloc(sizeof(Command*));
                size_t hold_size = 0;
                int spacing = command->spacing;
                for(;index<(int)size;index++){
                    Command* current = commands[index];
                    if(current->spacing <= spacing && current->declaration == T_NEW_LINE){break;}
                    hold = realloc(hold,sizeof(Command*)*(hold_size+1));
                    hold[hold_size++] = current;
                } 
                ERROR(hold_size == 0,command->line,(char*[]){"Block must have a command",NULL},__func__,scope);
                hold = parser_block(hold,hold_size,scope,&hold_size);
                for(int i=0;i<(int)hold_size;i++){
                    command->commands = realloc(command->commands,sizeof(Command*)*(command->command_size+1));
                    command->commands[command->command_size++] = hold[i];
                }
            }
            default:{
                blocks = realloc(blocks,sizeof(Command*)*(block_size+1));
                blocks[block_size++] = command;
                break;
            }
        }
    }
    *return_size = block_size;
    return blocks;
}
Command** parse(Token** tokens,size_t size,char* scope,size_t* return_size){
    Parser* parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->token_size = size;
    parser->scope = scope;
    parser->state = P_EMPTY;
    parser->command = malloc(sizeof(Command));
    //set defualt states
    parser_set_states(P_TYPE,parser_set_empty);

    parser_advance(parser);   
    while(parser->index<=parser->token_size){
        parser_function function = states[parser->state][parser->current->type];
        if(function!=NULL){function(parser);}
        else{parser_advance(parser);}
    }
    parser->commands = parser_block(parser->commands,parser->command_size,scope,&parser->command_size);
    *return_size = parser->command_size;
    return parser->commands;
}
