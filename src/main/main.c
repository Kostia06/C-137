#include <locale.h>

#include "../utils/include.h"
#include "../vector/include.h"

#include "../hash/include.h"
#include "../lexer/include.h"
#include "../parser/include.h"
#include "../para/include.h"
#include "../bakcend/include.h"

#define DEBUG_TOKEN     0
#define DEBUG_AST       0

int main(int argc, char *argv[]){
    CompilerOptions* options = parameters(argv,argc);
 
    setlocale(LC_CTYPE, "en_US.UTF-8");
    char* scope = options->file;
    char* text = READ_FILE(options->file);
    Vector* tokens = new_lexer(text,scope);
    #if DEBUG_TOKEN == 1
        printf("DEBUG TOKEN--------------------------------------\n");   
        for(int i =0;i<(int)tokens->size;i++){
            PRINT_TOKEN(vector_get(tokens,i));
        }
        printf("-------------------------------------------------\n");
    #endif
   
    parser_init();
    Vector* asts = parse(tokens,P_EMPTY,scope);
    #if DEBUG_AST == 1
        printf("DEBUG AST----------------------------------------\n");
        for(int i=0;i<(int)asts->size;i++){
            PRINT_AST(vector_get(asts,i),0);
        }
        printf("-------------------------------------------------\n");
    #endif

    backend_compile(asts,scope);

    return 0;
}
