#ifndef PARSER_AST_OP_PRIVATE_H
#define PARSER_AST_OP_PRIVATE_H

void op_complex(Parser* parser,AST* ast);
void op_simple(Parser* parser,AST* ast);
void op_complex_identifier(Parser* parser,AST* ast);
void add_negative_to_ast(Parser* parser,AST* ast);
void op_cycle(Parser* parser,AST* ast);
void op_cycle_identifier(Parser* parser,AST* ast);

#endif
