#include "luafunc.h"
#include "map.h"
#include "util.h"
#include "player.h"

int lua_dispError(lua_State* ls) {
    error(lua_tostring(ls, 1));
    return 0;
}

int lua_getTile(lua_State* ls) {
    return 0;
}

int lua_setTile(lua_State* ls) {
    map->tile_at(vec2(lua_tointeger(ls, 1), lua_tointeger(ls, 2)))->setSymbol({lua_tostring(ls, 3)[0], (uint16_t)lua_tointeger(ls, 4)});
    return 0;
}

int lua_message(lua_State* ls) {
    int color = 0;
    if(lua_gettop(ls) > 1)
        color = lua_tointeger(ls, 2);
    player->add_message(message(lua_tostring(ls, 1), color));
    return 0;
}

int lua_ask(lua_State* ls) {
    int color = 0;
    if(lua_gettop(ls) > 1)
        color = lua_tointeger(ls, 2);
    lua_pushboolean(ls, player->ask_question(message(lua_tostring(ls, 1), color)));
    return 1;
}
