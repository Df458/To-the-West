#include "effect.h"
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

int lua_effectAt(lua_State* ls) {
    std::string type = lua_tostring(ls, 1);
    if(type == "point") {
        map->addEffect(new Effect(vec2(lua_tonumber(ls, 2), lua_tonumber(ls, 3)), lua_toboolean(ls, 4), symbol(lua_tostring(ls, 5)[0], lua_tointeger(ls, 6)), lua_tostring(ls, 7), lua_tostring(ls, 8)));
    }
    else if(type == "radius") {
        map->addEffect(new Effect(vec2(lua_tonumber(ls, 2), lua_tonumber(ls, 3)), lua_tonumber(ls, 4), lua_toboolean(ls, 5), symbol(lua_tostring(ls, 6)[0], lua_tointeger(ls, 7)), lua_tostring(ls, 8), lua_tostring(ls, 9)));
    }
    else if(type == "line") {
        map->addEffect(new Effect(vec2(lua_tonumber(ls, 2), lua_tonumber(ls, 3)), vec2(lua_tonumber(ls, 4), lua_tonumber(ls, 5)), lua_toboolean(ls, 6), lua_toboolean(ls, 7), symbol(lua_tostring(ls, 8)[0], lua_tointeger(ls, 9)), lua_tostring(ls, 10), lua_tostring(ls, 11)));
    }
    return 0;
}

int lua_spawn(lua_State* ls) {
    map->spawnAt(vec2(lua_tointeger(ls, 1), lua_tointeger(ls, 2)), lua_tostring(ls, 3));
    return 0;
}
