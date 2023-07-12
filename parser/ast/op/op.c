#include "../../private.h"
#include "private.h"

#define STRING_NUM_SIZE 64

// just checks if ast is a normal number
static int is_normal_int(AST* ast){
    if(ast->children == NULL || ast->children->size == 0){return 1;}
    if(ast->children->size > 1){return 0;}
    AST* first = vector_get(ast->children,0);
    if(first->type == SUB){return 1;}
    return 0;
}
// check if the ast is a negative number
static int is_negative(AST* ast){
    if(ast->children == NULL || ast->children->size == 0){return 1;}
    AST* first = vector_get(ast->children,0);
    if(first->type == SUB){return -1;}
    return 1;
}
// do simple operation
static char* do_math(Parser* parser,AST* left,AST* right,int op){
    double value_1 = atof(left->value.string);
    value_1 *= is_negative(left);
    double value_2 = atof(right->value.string);
    value_2 *= is_negative(right);
    double value;
    switch(op){
        case ADD:{value = value_1 + value_2;break;} 
        case SUB:{value = value_1 - value_2;break;}
        case MUL:{value = value_1 * value_2;break;}
        case DIV:{value = value_1 / value_2;break;}
        case AND:{value = value_1 && value_2;break;}
        case OR:{value = value_1 || value_2;break;}
        case LESS:{value = value_1 < value_2;break;}
        case GREATER:{value = value_1 > value_2;break;}
        case LESS_EQUAL:{value = value_1 <= value_2;break;}
        case GREATER_EQUAL:{value = value_1 >= value_2;break;}
        case EQUAL_EQUAL:{value = value_1 == value_2;break;}
        case BANG_EQUAL:{value = value_1 != value_2;break;}
        case B_AND:{value = (int)value_1 & (int)value_2;break;}
        case B_OR:{value = (int)value_1 | (int)value_2;break;}
        case B_XOR:{value = (int)value_1 ^ (int)value_2;break;}
        case B_LEFT:{
            if(value_2<0){value_2 = -value_2;}
            if(value_1<0){value_1 = -value_1;}
            value = (int)value_1 << (int)value_2;
            break;
        }
        case B_RIGHT:{
            if(value_2<0){value_2 = -value_2;}
            if(value_1<0){value_1 = -value_1;}
            value = (int)value_1 >> (int)value_2;
            break;
        }
    }
    char* result = mem_init(parser->memory, STRING_NUM_SIZE*sizeof(char));
    snprintf(result,STRING_NUM_SIZE,"%.2f",value);
    return result;
}
// Simple operation with numbers
void op_simple(Parser* parser,AST* ast){
    if(ast->children == NULL || ast->children->size != 2){return;}
    AST* left = vector_get(ast->children,0);
    AST* right = vector_get(ast->children,1);
    if(left->type != INTEGER && left->type != FLOAT){return;}
    if(right->type != INTEGER && right->type != FLOAT){return;}
    char* result = do_math(parser,left,right,ast->type);
    ast->type = (left->type == FLOAT || right->type == FLOAT)?FLOAT:INTEGER;
    ast->actual_type = ast->type;
    ast->value.string = result;
    // clean up
    parser_free_ast(parser->memory,left); 
    parser_free_ast(parser->memory,right);
    vector_set_size(ast->children,0);
}
// complex operation with numbers
void op_complex(Parser* parser,AST* ast){
    if(ast->children == NULL || ast->children->size != 2){return;}
    // simple left and right sides of the operation
    AST* op = vector_get(ast->children,0);
    AST* value = vector_get(ast->children,1);
    if(op->type != SUB && op->type != ADD){return;}
    if(value->type != INTEGER && value->type != FLOAT){return;}
    // the value of the second operation
    AST* op_value = vector_get(op->children,1);
    if(op_value == NULL || (op_value->type != INTEGER && op_value->type != FLOAT)){return;}
    if(op->type == SUB){add_negative_to_ast(parser,op_value);}
    // do the math 
    char* result = do_math(parser,op_value,value,ast->type);
    mem_free(parser->memory,op_value->value.string);
    op_value->type = (op_value->type == FLOAT || value->type == FLOAT)?FLOAT:INTEGER;
    op_value->actual_type = op_value->type;
    op_value->value.string = result;
    if(op_value->children != NULL){
        mem_free(parser->memory,vector_get(op_value->children,0));
        vector_set_size(op_value->children,0);
    }
    if(op->type == SUB){op->type = ADD;op->actual_type = ADD;}
    // clean up
    parser_replace_ast(ast,op);
    parser_free_ast(parser->memory,value);
    vector_set_size(op->children,0);
    parser_free_ast(parser->memory,op);
}
// checks if the ast is a definition or identifier
static AST* check_definition(AST* ast, int* is_mul){
    if(ast == NULL){return NULL;}
    if(ast->type == IDENTIFIER){return ast;}
    // check if the ast is a definition EX: 2x or 5y
    if(ast->type != MUL|| ast->children == NULL || ast->children->size != 2){return NULL;}
    AST* left = vector_get(ast->children,0);
    AST* right = vector_get(ast->children,1);
    if(left->type != IDENTIFIER){
        // give the position of the identifier
        *is_mul = 0;return left;
    }
    if(right->type != IDENTIFIER){
        // give the position of the identifier
        *is_mul = 1; return right;
    }
    return NULL;
}
static void destroy_definition(Parser* parser,AST* ast,AST* op,AST* op_value,AST* value, int pos){
    AST* last = vector_get(op->children,pos);
    parser_free_ast(parser->memory,last);
    vector_set_size(op->children,op->children->size-1);
    
    last = vector_get(ast->children,1);
    vector_set_size(ast->children,ast->children->size-1);
    
    parser_replace_ast(ast,vector_get(op->children,0));
    // clean up
    parser_free_ast(parser->memory,last);
}
static void new_definition(Parser* parser,AST* ast,AST* op,AST* op_value,AST* value,int sign,int pos, int negative){
    if(is_negative(op_value)==-1){sign = !sign;negative = 1;}
    // if the sign is negative then destory both variables since they cancel each other out
    if(sign){destroy_definition(parser,ast,op,op_value,value,pos);return;}
    // declare new definition
    AST* new_op = parser_new_ast(parser, MUL,1);
    AST* new_size = parser_new_ast(parser, INTEGER,0);
    new_size->value.string = negative ? "-2" : "2";
    // add 2 children
    vector_add(new_op->children,new_size);
    vector_add(new_op->children,value);
    parser_replace_ast(op_value,new_op);
    parser_replace_ast(ast,op);
    // clean up
    parser_free_ast(parser->memory,new_op);
    mem_free(parser->memory,op);
}
static void definition_to_variable(Parser* parser,AST* ast, AST* op,AST* value, int is_mul, int negative){
    AST* last = vector_get(op->children,is_mul == 0 ? 0: 1);
    // make the value positive & clean up
    if(negative){
        mem_free(parser->memory,vector_get(value->children,0));
        vector_set_size(value->children,0);
    }
    parser_replace_ast(last,value);
    parser_replace_ast(ast,op);
    // clean up
    parser_free_ast(parser->memory,value);
    vector_set_size(op->children,0);
    parser_free_ast(parser->memory,op);
}
static void change_definition(Parser* parser,AST* ast,AST* value,AST* op,AST* op_value, double num){
    char* result = mem_init(parser->memory, STRING_NUM_SIZE*sizeof(char));
    snprintf(result,STRING_NUM_SIZE,"%.2f",num);
    // destroy old value
    mem_free(parser->memory,op_value->value.string);
    op_value->value.string = result;
    parser_replace_ast(ast,op);
    // clean up 
    vector_set_size(op->children,0);
    parser_free_ast(parser->memory,op);
    parser_free_ast(parser->memory,value);
}
// complex operation with identifier
void op_complex_identifier(Parser* parser,AST* ast){
    if(ast->children == NULL || ast->children->size != 2){return;}
    // simple left and right of the current op
    AST* op = vector_get(ast->children,0);
    AST* value = vector_get(ast->children,1);
    if(value->type != IDENTIFIER || !is_normal_int(value)){return;}
    if(op->children == NULL || op->children->size != 2){return;}
    // get the identifier
    AST* op_value;
    int pos = 0, is_mul = -1;
    // check if left or right is a identifier or definition
    AST* left_op = check_definition((AST*)vector_get(op->children,0),&is_mul); 
    AST* right_op = check_definition((AST*)vector_get(op->children,1),&is_mul);
    if(left_op != NULL){op_value = left_op;}
    else if(right_op != NULL){op_value = right_op;pos = 1;}
    else{return;} // if both are not identifier or definition, return
    if(strcmp(op_value->value.string, value->value.string) != 0){return;}
    // check if we need to add or subtract
    int sign = 0; // 0 == ADD 1 == SUB
    int negative = 0;
    if(op->type == SUB){sign = !sign;}
    if(is_negative(value)==-1){sign = !sign;negative = 1;}
    if(ast->type == SUB){sign = !sign;}
    // declare new definition 
    if(is_mul==-1){new_definition(parser,ast,op,op_value,value,sign,pos,negative);return;}
    // work with old definition
    int add = sign == 0 ? 1: -1;
    double num = atof(op_value->value.string) ;
    num += add;
    if(num == 1){definition_to_variable(parser,ast,op,value,is_mul,negative);}
    else{change_definition(parser,ast,value,op,op_value,num);}
}
