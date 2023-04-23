#include "../utils/include.h"
#include "../hash/include.h"
#include "../lexer/include.h"
#include "../parser/include.h"
#include "../para/include.h"

#define DEBUG_TOKEN     0
#define DEBUG_COMMAND   0

int main(int argc, char *argv[]){
    CompilerOptions* options = parameters(argv,argc);

    Lexer* lexer = new_lexer(options);

    Token** tokens = lexer->tokens;
    size_t token_size = lexer->token_size;
    #if DEBUG_TOKEN == 1
         for(int i =0;i<(int)token_size;i++){PRINT_TOKEN(tokens[i],0);}
    #endif
    
    size_t command_size = 0;
    Command** commands = parse(tokens,token_size,options->file,&command_size);
    #if DEBUG_COMMAND == 1
        for(int i=0;i<(int)command_size;i++){PRINT_COMMAND(commands[i],0);}
    #endif
    return 0;
}
