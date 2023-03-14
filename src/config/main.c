#include "include.h"

char* read_config(char* file, char* table, char* key){
    ERROR(!LUA_IS_INCLUDED,0,(char*[]){"Lua is not installed",NULL},__func__,"PUBLIC");
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L,file);
    lua_getglobal(L,table);
    lua_getfield(L,-1,key);
    char* value = (char*)lua_tostring(L,-1);
    char* result = malloc(strlen(value)+1);
    strcpy(result,value);
    lua_close(L);
    return result;
}
int check_config(char* file){
    ERROR(!LUA_IS_INCLUDED,0,(char*[]){"Lua is not installed",NULL},__func__,"PUBLIC");
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    int result = luaL_dofile(L,file)==LUA_TNIL ? 1 : 0;
    lua_close(L);
    return result;
}
int check_config_table(char* file, char* table){
    ERROR(!LUA_IS_INCLUDED,0,(char*[]){"Lua is not installed",NULL},__func__,"PUBLIC");
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    ERROR(luaL_dofile(L,file),0,(char*[]){"Config file does not exist",NULL},__func__,"PUBLIC");
    lua_getglobal(L,table);
    int result = lua_istable(L,-1) ==LUA_TNIL ? 0 : 1;
    lua_close(L);
    return result;
}
int check_config_key(char* file, char* table, char* key){
    ERROR(!LUA_IS_INCLUDED,0,(char*[]){"Lua is not installed",NULL},__func__,"PUBLIC");
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    ERROR(luaL_dofile(L,file),0,(char*[]){"Config file does not exist",NULL},__func__,"PUBLIC");
    lua_getglobal(L,table);
    int result = lua_getfield(L,-1,key) ==LUA_TNIL ? 0 : 1;
    lua_close(L);
    return result;
}