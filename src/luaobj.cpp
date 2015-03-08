#include "util.h"
#include "luaobj.h"

void LuaObject::call(std::string name) {
    if(name.empty())
        return;

	lua_newtable(game_state);
	insert();
    lua_setglobal(game_state, "this");
    luaL_loadfile(game_state, (get_path() + "/data/scripts/" + name).c_str());
	if(lua_pcall(game_state, 0, LUA_MULTRET, 0)) {
        error(lua_tostring(game_state, -1));
    } else {
        lua_getglobal(game_state, "this");
        retrieve();
    }
}

void LuaObject::insert() {
}

void LuaObject::retrieve() {
}
