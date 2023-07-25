#include "../private.h"

// hadles special cases of signs
static int lexer_special_sign(Lexer* lexer){
    switch(lexer->node->type){
        case SINGLE_COMMENT_START:{
            lexer->node->type = SINGLE_COMMENT;
            return 1;
        }
        case MULTI_COMMENT_START:{
            lexer->node->type = MULTI_COMMENT;
            return 1;
        }
    }
    return 0;
}
// just finds the sign type
static int lexer_get_sign_type(Lexer* lexer){
    char* name = lexer->node->value.string;
    for(int i=0;i<SIGN_COUNT;i++){
        if(!strcmp(name,signs[i].name)){return signs[i].type;}
    }
    return EMPTY;
}
// Start of a sign
void lexer_new_sign(Lexer* lexer){
    lexer->node->type = SIGN;
    lexer->node->index = lexer->index;
    lexer->node->value.string = mem_init(lexer->compiler->memory,sizeof(char)*2);
}
// Add a char to the sign and check if it is a sign
void lexer_add_char_to_sign(Lexer* lexer){
    int current_char_type = characters[lexer->current_char];
    while(current_char_type == LT_SIGN && lexer->node->size < LONGEST_SIGN ){
        lexer_add_char(lexer);
        current_char_type = characters[lexer->current_char];
    }
    int size = strlen(lexer->node->value.string);
    for(int i = size;i>0;i--){
        int type = lexer_get_sign_type(lexer);
        if(i == 0 && type == EMPTY){
            char* message = SYNC((char*[]){"Invalid sign",NULL});
            error_single_init(
                lexer->compiler->error,
                SYNTAX_ERROR,
                lexer->node->index,
                lexer->node->index+1,
                message
            );
        }
        else if(type != EMPTY){
            lexer->node->type = type;
            lexer->node->size = i;
            mem_free(lexer->compiler->memory,lexer->node->value.string);
            int special = lexer_special_sign(lexer);
            if(!special){lexer_reset_node(lexer);}
            return;
        }
        lexer->node->value.string = realloc(lexer->node->value.string,--size);
        lexer->node->value.string[size] = '\0';
        lexer_back(lexer);
    }
}
