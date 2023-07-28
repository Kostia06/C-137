#include "include.h"
#include "private.h"

#define INDEX       500000

Node* new_empty_node(MemoryGroup* memory){
    Node* node = mem_init(memory,sizeof(Node));
    node->type = EMPTY;
    return node;
}
// Add the current node to the vector and create a new node
void lexer_reset_node(Lexer* lexer){
    vector_add(lexer->nodes,lexer->node);
    lexer->node = new_empty_node(lexer->compiler->memory);
}
// Gets the next character from the file
void lexer_advance(Lexer* lexer){
    lexer->index++;
    lexer->current_char = lexer->text[lexer->index];
}
// Returns the previous character from the file 
void lexer_back(Lexer* lexer){
    lexer->index--;
    lexer->current_char = lexer->text[lexer->index];
}
void lexer_skip(Lexer* lexer){
    lexer_advance(lexer);
    lexer_advance(lexer);
}
char lexer_peek(Lexer* lexer){
    return lexer->text[lexer->index+1]; 
}
Vector* new_lexer(Compiler* compiler,char* text){
    if(text == NULL){return NULL;}
    Lexer* lexer = mem_init(compiler->memory,sizeof(Lexer));
    lexer->compiler = compiler;
    // Vector of nodes and current node
    lexer->nodes = vector_init(lexer->compiler->memory);
    lexer->node = new_empty_node(compiler->memory);
    lexer->node->type = NEW_LINE;
    // Get the file
    lexer->text = text;
    lexer->text_size = strlen(text);

    lexer->current_char = lexer->text[0];
   
    int debug_index = 0;
    while(lexer->index < lexer->text_size && debug_index++ < INDEX){
        // Variable checks if the current character is a emoji
        // Every emoji is a array of negative numbers
        int type = LT_ALPHA;
        // if the current character is not a emoji get the character type from the array
        if(lexer->current_char >= 0){type = characters[lexer->current_char];}
        // Get the function that corresponds to the current character type
        lexer_function function = states[lexer->node->type][type];
        function(lexer);
    }
    if(debug_index >= INDEX){
        char* message = SYNC((char*[]){"Infinite loop",NULL});
        error_single_init(
            lexer->compiler->error,
            SYNTAX_ERROR,
            lexer->node->index,
            lexer->node->index+1,
            message
        );
        return NULL;
    }
    // If the string is not closed
    if(lexer->node->type != NEW_LINE){
        char* message = SYNC((char*[]){"String not closed",NULL});
        error_single_init(
            lexer->compiler->error,
            SYNTAX_ERROR,
            lexer->node->index,
            lexer->node->index+1,
            message
        );
    }
    // get the last node
    lexer_reset_node(lexer);

    Vector* nodes = lexer->nodes;
    mem_free(compiler->memory,lexer->node);
    mem_free(compiler->memory,lexer->text);
    mem_free(compiler->memory,lexer);
    return nodes;
}
