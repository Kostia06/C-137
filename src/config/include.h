#ifndef CONFIG_H
#define CONFIG_H

#if  __has_include("lua.h")
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
    #define LUA_IS_INCLUDED     1
#else
    #define LUA_IS_INCLUDED     0
#endif

#include "../utils/include.h"

void check_lua();
char* read_config(char* file, char* table, char* key);
int check_config(char* file);
int check_config_table(char* file, char* table);
int check_config_key(char* file, char* table, char* key);
#endif
