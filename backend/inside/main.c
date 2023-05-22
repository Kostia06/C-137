#include "../type.h"

void backend_loop_inside(Backend* backend, char* name){
    char* scope = malloc(strlen(backend->current_scope)+1);
    strcpy(scope,backend->current_scope);
    strcat(backend->current_scope,"!~!");
    strcat(backend->current_scope,name);
    for(int i=0;i<(int)backend->command->command_size;i++){
        backend->command = backend->command->commands[i];
        backend_function function = inside_states[backend->command->type];
        function(backend);
    }
    backend->current_scope = scope;
}
