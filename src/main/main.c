#include "../utils/include.h"

#include "../flags/include.h"
#include "../lexer/include.h"
#include "../parser/include.h"
#include "../ir/include.h"

#define DEBUG_LEX       0
#define DEBUG_PARSE     1
#define MEMORY_AUTO_USED  6
/*
    # always takes memory, a friendly reminder
    1. compiler error group
    2. compiler scopes vector
    3. compiler flags vector
    4. compiler swaps vector + everything inside
    5. compiler flags files
    6. the vector of nodes
*/

static Compiler* compiler_init(){
    Compiler* compiler = malloc(sizeof(Compiler));
    compiler->memory = mem_group_init();
    compiler->scopes = vector_init(compiler->memory);
    compiler->swaps = vector_init(compiler->memory);
    return compiler;
}
static int memory_match_check(Compiler* compiler, Vector* nodes){
    int count = MEMORY_AUTO_USED;
    for(int i=0;i<(int)nodes->size;i++){count += NODE_MEMORY(vector_get(nodes,i));}
    for(int i=0;i<(int)compiler->swaps->size;i++){count += NODE_MEMORY(vector_get(compiler->swaps,i));}
    return count == compiler->memory->size;
}
static Vector* lex(Compiler* compiler){
    char* text = READ_FILE(compiler->error,compiler->memory,compiler->scope);
    Vector* nodes = new_lexer(compiler,text);
    #if DEBUG_LEX
        printf("LEXER--------------------------------------------------------\n");   
        for(int i=0;i<(int)nodes->size;i++){
            Node* node = vector_get(nodes,i);
            PRINT_NODE(node,0);
        }
        if(!memory_match_check(compiler,nodes)){
            char* message = SYNC((char*[]){"Memory match check failed in lexer",NULL}); 
            error_single_init(compiler->error,MEMORY_ERROR,0,0,message);
        }
    #endif
    error_execute(compiler->error);
    return nodes;
}
static Vector* parse(Compiler* compiler,Vector* nodes){
    Vector* asts = new_parser(compiler,nodes,EMPTY);
    #if DEBUG_PARSE 
        printf("PARSER-------------------------------------------------------\n");
        for(int i=0;i<(int)asts->size;i++){
            Node* ast = vector_get(asts,i);
            PRINT_NODE(ast,0);
        }
        if(!memory_match_check(compiler,asts)){
            char* message = SYNC((char*[]){"Memory match check failed in parser",NULL}); 
            error_single_init(compiler->error,MEMORY_ERROR,0,0,message);
        }
    #endif
    error_execute(compiler->error);
    return asts;
}
static void ir(Compiler* compiler,Vector* asts){
    new_ir(compiler,asts);
    error_execute(compiler->error);
}


int main(int argc, char *argv[]){
    Compiler* compiler = compiler_init(); 
    CompilerFlags* flags = flags_init(compiler->memory,argv,argc);
    setlocale(LC_CTYPE, "en_US.UTF-8");
    for(int i=0;i<(int)flags->files->size;i++){
        // set up current scope
        char* scope = vector_get(flags->files,i);
        vector_add(compiler->scopes,scope);
        compiler->scope = scope;
        // set up new error group
        compiler->error = error_group_init(compiler); 
        // lex the file
        Vector* nodes = lex(compiler);
        // create asts from the nodes
        Vector* asts = parse(compiler,nodes);
        // create ir from the asts
        ir(compiler,asts);
    }
    return 0;
}
