#include "../utils/include.h"
#include "../hash/include.h"
#include "../lexer/include.h"
#include "../parser/include.h"
#include "../para/include.h"
#include "../stack/include.h"
#include "../vm/include.h"
#include "../bytecode/include.h"

#define DEBUG_TOKEN     1
#define DEBUG_AST       0
#define DEBUG_BYTECODE  1

int main(int argc, char *argv[]){
    HashTable* table = new_hash_table();
    Stack* stack = new_stack();

    CompilerOptions* options = parameters(argv,argc);

    Lexer* lexer = new_lexer(options);
    return 0 ;
    lex(lexer);
    hash_create_scope(table,options->file);

    Token** tokens = lexer->tokens;
    size_t token_size = lexer->token_size;
    #if DEBUG_TOKEN == 1
         for(int i =0;i<(int)token_size;i++){PRINT_TOKEN(tokens[i],0);}
    #endif
    /*
    tokens = create_macro(tokens,token_size,path,&token_size);
    
    tokens = ast(tokens,token_size,path,&token_size);

    #if DEBUG_AST == 1
        for(int i =0;i<(int)token_size;i++){PRINT_TOKEN(tokens[i],0);}
    #endif
    
    size_t bytecode_size = 0;
    Bytecode** bytecodes = build_bytecodes(table,tokens,token_size,path,&bytecode_size);

    #if DEBUG_BYTECODE == 1
        for(int i =0;i<(int)bytecode_size;i++){PRINT_BYTECODE(bytecodes[i],0);}
    #endif
    */
    return 0;
}
