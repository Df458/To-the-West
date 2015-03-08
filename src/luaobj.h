#ifndef LUA_OBJ_H
#define LUA_OBJ_H
#include <string>
extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}

extern lua_State* game_state;

class LuaObject {
public:
    void call(std::string);
    virtual void insert(void);
    virtual void retrieve(void);
};
#endif
