#ifndef LUA_OBJ_H
#define LUA_OBJ_H
#include <string>
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

extern lua_State* game_state;

class LuaObject {
public:
    void call(std::string);
    virtual void insert(void);
    virtual void retrieve(void);
};
#endif
