#include "../utils/include.h"
#include "../hash/include.h"
#include "../lexer/include.h"
#include "../parser/include.h"
#include "../para/include.h"

#define DEBUG_TOKEN     0
#define DEBUG_MACRO     0
#define DEBUG_AST       1


int main(int argc, char *argv[]){
    char* path = parameters(argv,argc);
    HashTable* table = malloc(sizeof(HashTable));
    table->size = 0; 
    Lexer* lexer = new_lexer(path);
    lex(lexer);

    Token** tokens = lexer->tokens;
    size_t token_size = lexer->token_size;
    #if DEBUG_TOKEN == 1
         for(int i =0;i<(int)token_size;i++){PRINT_TOKEN((Token*)tokens[i],0);}
    #endif

    tokens = create_macro(tokens,token_size,path,&token_size);
    #if DEBUG_MACRO == 1
        for(int i =0;i<(int)token_size;i++){PRINT_TOKEN((Token*)tokens[i],0);}
    #endif
    
    tokens = ast(tokens,token_size,path,&token_size);

    #if DEBUG_AST == 1
        for(int i =0;i<(int)token_size;i++){PRINT_TOKEN((Token*)tokens[i],0);}
    #endif

    hash_print(table);
    return 0;
}
