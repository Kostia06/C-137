#include "include.h"
#include "private.h"

Node* new_empty_node(MemoryGroup* memory){
    Node* node = mem_init(memory,sizeof(Node));
    node->type = EMPTY;
    return node;
}
// Add the current node to the vector and create a new node
void lexer_reset_node(Lexer* lexer){
    vector_add(lexer->nodes,lexer->node);
    lexer->node = new_empty_node(lexer->memory);
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
char lexer_peek(Lexer* lexer){
    return lexer->text[lexer->index+1]; 
}
Vector* new_lexer(ErrorGroup* error,MemoryGroup* memory,char* text, char* scope){
    if(text == NULL){return NULL;}
    Lexer* lexer = mem_init(memory,sizeof(Lexer));
    // Memory & error group
    lexer->memory = memory;
    lexer->error = error;
    // Vector of nodes and current node
    lexer->nodes = vector_init();
    lexer->node = new_empty_node(memory);
    lexer->node->type = NEW_LINE;
    // Get the file
    lexer->text = text;
    lexer->text_size = strlen(text);

    lexer->scope = scope;
    lexer->current_char = lexer->text[0];
    
    while(lexer->index < lexer->text_size){
        // Variable checks if the current character is a emoji
        // Every emoji is a array of negative numbers 
        int type = LT_ALPHA;
        // if the current character is not a emoji get the character type from the array
        if(lexer->current_char >= 0){type = characters[lexer->current_char];}
        // Get the function that corresponds to the current character type
        lexer_function function = states[lexer->node->type][type];
        function(lexer);
    
    }
    // If the string is not closed
    if(lexer->node->type != NEW_LINE){
        char* message = SYNC((char*[]){"String not closed",NULL});
        error_single_init(lexer->error,SYNTAX_ERROR,lexer->node->index,lexer->node->index+1,message);
    }
    // get the last node
    lexer_reset_node(lexer);

    Vector* nodes = lexer->nodes;
    mem_free(memory,lexer->node);
    mem_free(memory,lexer->text);
    mem_free(memory,lexer);
    return nodes;
}
