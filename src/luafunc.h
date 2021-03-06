#ifndef LUA_FUNC_H
#define LUA_FUNC_H
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

int lua_dispError(lua_State* ls);
int lua_getTile(lua_State* ls);
int lua_setTile(lua_State* ls);
int lua_message(lua_State* ls);
int lua_ask(lua_State* ls);
int lua_effectAt(lua_State* ls);
int lua_spawn(lua_State* ls);
int lua_win(lua_State* ls);

const luaL_Reg lua_global_functions[] = {
    {"disperror", lua_dispError},
    {"gettile", lua_getTile},
    {"settile", lua_setTile},
    {"message", lua_message},
    {"ask", lua_ask},
    {"effect", lua_effectAt},
    {"spawn", lua_spawn},
    {"win", lua_win},
    {0, 0}
};
#endif
