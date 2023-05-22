#include "include.h"
#include "private.h"

#define DEBUG_RULE    0

static int keyword_offset = 0;
static int op_offset = 0;
static int type_offset = 0;

static void parser_set_rule_functions(int type,parser_function function){
    parser_function* functions = rule_functions[type]; 
    for(int i=0;i<P_END;i++){
        if(functions[i] == NULL){functions[i] = function;}
    }
}
static char* parser_get_values_from_rule(Rule rule){
    if(rule.size == 0){return "NOTHING";}
    char* values = malloc(sizeof(char));
    int size = rule.size > 2 ? 2  : rule.size;
    for(int i=0;i<size;i++){
        char* value = malloc(sizeof(char)*strlen(PARSER_TYPE(rule.values[i])));
        strcpy(value,PARSER_TYPE(rule.values[i]));
        strcat(values,value);
        if(i == size-2){
            strcat(values," or ");
        }
        else if(i+1 < size){
            strcat(values,", ");
        }
    }
    values[strlen(values)] = '\0';
    return values;
}
static void parser_read_rule(Parser* parser,RuleSet rule_set){
    int index = parser->main_ast->children->size;
    if(rule_set.size == -1){index = 0;}
    {
        char* error[] = {"Rule not implemented for ",PARSER_TYPE(parser->ast->type)," in ",PARSER_TYPE(parser->main_ast->type),NULL};
        int e_condition = index >= rule_set.size && rule_set.size != -1;
        ERROR(e_condition,parser->ast->line,parser->ast->column,parser->ast->size,error,parser->scope);

    }
    Rule current_rule = rule_set.rules[index];
    int found = 0, i=-1;
    for(;i<current_rule.size;++i){
        #if DEBUG_RULE == 1
            printf("%s-%d\t",PARSER_TYPE(parser->main_ast->type),(int)parser->main_ast->children->size);
            printf("%s\t%s\n",PARSER_TYPE(current_rule.values[i]),PARSER_TYPE(parser->ast->type));
        #endif
        if(current_rule.values[i] == parser->ast->type){found = 1;break;}
    }
    if(!found){
        char* values = parser_get_values_from_rule(current_rule);
        char* error[] = {"Expected ",values,", but got ",PARSER_TYPE(parser->ast->type)," in ",PARSER_TYPE(parser->main_ast->type),NULL};
        ERROR(1,parser->ast->line,parser->ast->column,parser->ast->size,error,parser->scope);
    }
    parser_function function = rule_functions[parser->main_ast->type][parser->ast->type];
    {
        char* error[] = {PARSER_TYPE(current_rule.values[i])," has not rule in ",PARSER_TYPE(parser->main_ast->type),NULL};
        int e_condition = function == NULL;
        ERROR(e_condition,parser->ast->line,parser->ast->column,parser->ast->size,error,parser->scope);
    }
    function(parser);
}



void parser_init(){
    keyword_offset = abs(T_FN-P_FN);
    op_offset = abs(T_ADD-P_ADD);
    type_offset = abs(T_I1-P_I1);
    if(P_FN < T_FN){keyword_offset = -keyword_offset;}
    if(P_ADD < T_ADD){op_offset = -op_offset;}
    if(P_I1 < T_I1){type_offset = -type_offset;}
}
Vector* parse(Vector* tokens,int type,char* scope){
    Parser* parser = malloc(sizeof(Parser));
    parser->scope = scope;
    parser->init_type = type;
    parser->tokens = tokens;
    parser->main_ast = malloc(sizeof(AST));
    parser->ast = malloc(sizeof(AST));
    parser->asts = vector_init();
    parser->blocks = vector_init();
    parser->main_ast->children = vector_init();
    parser_advance(parser);
    parser->main_ast->type = type;
    parser->main_ast->actual_type = type;
    parser->keyword_offset = keyword_offset;
    parser->op_offset = op_offset;
    parser->type_offset = type_offset;
    while(parser->index <= parser->tokens->size){
        parser_function function= build_ast_token[parser->token->type];
        {
            char* error[] = {"Invalid token, ",TOKEN_TYPE(parser->token->type),", in ",PARSER_TYPE(parser->main_ast->type),NULL};
            int e_condition = function == NULL;
            ERROR(e_condition,parser->token->line,parser->token->column,parser->token->size,error,parser->scope);
        }
        function(parser);
        RuleSet rule_set = rules[parser->main_ast->type];
        if(parser->ast->type != P_EMPTY){
            parser_read_rule(parser,rule_set);
        }
    }
    return parser->asts;
}
