#include "../type.h"

void backend_error(Backend* backend){
    ERROR(1,backend->command->line,(char*[]){"\"",PARSER_TYPE(backend->command->type),"\" is not allowed in this scope",NULL},backend->current_scope);
}
