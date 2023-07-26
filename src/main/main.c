#include "../utils/include.h"

#include "../flags/include.h"
#include "../lexer/include.h"
#include "../parser/include.h"
#include "../ir/include.h"

#define DEBUG_LEX       0
#define DEBUG_PARSE     1

static Compiler* compiler_init(){
    Compiler* compiler = malloc(sizeof(Compiler));
    compiler->memory = mem_group_init();
    compiler->scopes = vector_init(compiler->memory);
    return compiler;
}
static Vector* lex(Compiler* compiler){
    char* text = READ_FILE(compiler->error,compiler->memory,compiler->scope);
    Vector* nodes = new_lexer(compiler,text);
    error_execute(compiler->error);
    #if DEBUG_LEX
        printf("LEXER--------------------------------------------------------\n");   
        for(int i=0;i<(int)nodes->size;i++){
            Node* node = vector_get(nodes,i);
            PRINT_NODE(node,0);
        }
        mem_print(compiler->memory);
    #endif
    return nodes;
}
static Vector* parse(Compiler* compiler,Vector* nodes){
    Vector* asts = new_parser(compiler,nodes,EMPTY);
    error_execute(compiler->error);
    #if DEBUG_PARSE 
        printf("PARSER-------------------------------------------------------\n");
        for(int i=0;i<(int)asts->size;i++){
            Node* ast = vector_get(asts,i);
            PRINT_NODE(ast,0);
        }
        mem_print(compiler->memory);
    #endif
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
    /*
        # always takes memory, a friendly reminder
        1. compiler error group
        2. compiler scopes 
        3. compiler flags
        4. compiler flags files
        5. the vector of nodes
    */
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
