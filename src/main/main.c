#include "../utils/include.h"
#include "../hash/include.h"
#include "../lexer/include.h"
#include "../parser/node/include.h"
#include "../parser/macro/include.h"
#include "../parser/block/include.h"
#include "../compile/include.h"
#include "../config/include.h"

#define DEBUG_TOKEN     0
#define DEBUG_NODE      0
#define DEBUG_MACRO     0
#define DEBUG_BLOCK     0

Config* new_config(int argc, char *argv[]){
    Config* config = malloc(sizeof(Config));
    config->input_file = "";
    config->output_folder = "output.c";
    config->c_flags = "";
    config->compiler_mode = "compile";
    config->configured = 0;
    if(argc == 1){
        check_lua();
        ERROR(!check_config(CONFIG_FILE),0,(char *[]){"Config file not found",NULL},__func__,"PUBLIC");
        ERROR(!check_config_table(CONFIG_FILE,"config"),0,(char *[]){"Table in the config file is not found",NULL},__func__,"PUBLIC");
        if(check_config_key(CONFIG_FILE,"config","_input_file")){
            config->input_file = read_config(CONFIG_FILE,"config","_input_file");
        }
        if(check_config_key(CONFIG_FILE,"config","_output_folder")){
            config->output_folder = read_config(CONFIG_FILE,"config","_output_folder");
        }
        if(check_config_key(CONFIG_FILE,"config","_c_flags")){
            config->c_flags = read_config(CONFIG_FILE,"config","_c_flags");
        }
        if(check_config_key(CONFIG_FILE,"config","_compiler_mode")){
            config->compiler_mode = read_config(CONFIG_FILE,"config","_compiler_mode");
        }
        config->configured = 1;
    }
    else{
        if(argc > 2){config->input_file = argv[1];}
        if(argc > 3){config->output_folder = argv[2];}
        if(argc > 4){config->c_flags = argv[3];}
        if(argc > 5){config->compiler_mode = argv[4];}
    }
    return config;
}

int main(int argc, char *argv[]){
    Config* config = new_config(argc,argv);
    int path_size = 0;

    HashTable* table = malloc(sizeof(HashTable));
    table->size = 0;
    Lexer* lexer = new_lexer(table,config);
    hash_scope_init(table,config->input_file);
    lex(lexer);

    size_t node_size = 0;
    Node** nodes = build_nodes(lexer->tokens,"",lexer->token_size,&node_size);
    #if DEBUG_NODE == 1
        for(int i =0;i<(int)node_size;i++){PRINT_NODE(nodes[i],0);}
    #endif

    size_t macro_size = 0;
    Node** macro = create_macro(nodes,node_size,config->input_file,&macro_size);
    #if DEBUG_MACRO == 1
        for(int i =0;i<(int)macro_size;i++){PRINT_NODE(macro[i],0);}
    #endif

    size_t block_size = 0 ;
    NodeBlock** blocks = create_blocks(macro,macro_size,config->input_file,&block_size);
    #if DEBUG_BLOCK == 1
        for(int i =0;i<(int)block_size;i++){PRINT_BLOCK(blocks[i],0);}
    #endif

    compile(table,config,blocks,block_size);

    hash_print(table);
    return 0;
}
