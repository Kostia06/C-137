#include "../utils/include.h"

#include "../flags/include.h"
#include "../lexer/include.h"
#include "../parser/include.h"

#define DEBUG_LEX       0
#define DEBUG_PARSE     1

static Vector* lex(MemoryGroup* node_memory,char* file){
    ErrorGroup* node_error = error_group_init(file);
    char* text = READ_FILE(node_error,node_memory,file);
    Vector* nodes = new_lexer(node_error,node_memory,text,file);
    error_execute(node_error);
#if DEBUG_LEX
    printf("LEXER-------------------------------------------------------\n");   
    for(int i=0;i<(int)nodes->size;i++){
        Node* node = vector_get(nodes,i);
        PRINT_NODE(node,0);
    }
    mem_print(node_memory);
#endif
    // clean up
    error_free(node_error);
    return nodes;
}
static Vector* parse(MemoryGroup* node_memory,Vector* nodes, char* file){
    ErrorGroup* ast_error = error_group_init(file);
    Vector* asts = new_parser(ast_error,node_memory,nodes,EMPTY,file);
    error_execute(ast_error);
#if DEBUG_PARSE 
    printf("PARSER-------------------------------------------------------\n");
    for(int i=0;i<(int)asts->size;i++){
        Node* ast = vector_get(asts,i);
        PRINT_NODE(ast,0);
    }
    mem_print(node_memory);
#endif
    // clean up 
    error_free(ast_error);    
    return asts;
}

int main(int argc, char *argv[]){
    MemoryGroup* flags_memory = mem_group_init();
    CompilerFlags* flags = flags_init(flags_memory,argv,argc);
    // support for utf-8 aka emojis and other languages
    setlocale(LC_CTYPE, "en_US.UTF-8");
    for(int i=0;i<(int)flags->files->size;i++){
        char* file = vector_get(flags->files,i);
        // lex the file
        MemoryGroup* node_memory = mem_group_init();
        Vector* nodes = lex(node_memory,file);
        // create asts from the nodes
        Vector* asts = parse(node_memory,nodes,file);
    }
    return 0;
}
