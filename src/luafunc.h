#ifndef LUA_FUNC_H
#define LUA_FUNC_H
extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}

int lua_dispError(lua_State* ls);
int lua_getTile(lua_State* ls);
int lua_setTile(lua_State* ls);
int lua_message(lua_State* ls);

const luaL_reg lua_global_functions[] = {
    {"disperror", lua_dispError},
    {"gettile", lua_getTile},
    {"settile", lua_setTile},
    {"message", lua_message},
    {0, 0}
};
#endif
