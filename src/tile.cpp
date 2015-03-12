#include "tile.h"
#include <curses.h>

void Tile::draw(WINDOW* win, int x, int y) {
    wattron(win, COLOR_PAIR(displayed.color_pair));
    mvwaddch(win, y, x, displayed.disp);
    wattroff(win, COLOR_PAIR(displayed.color_pair));
}

void Tile::insert() {
    lua_pushinteger(game_state, position.x);
    lua_setfield(game_state, -2, "x");
    lua_pushinteger(game_state, position.y);
    lua_setfield(game_state, -2, "y");

    lua_pushboolean(game_state, passable);
    lua_setfield(game_state, -2, "passable");
    lua_pushboolean(game_state, occupied);
    lua_setfield(game_state, -2, "occupied");
    lua_pushstring(game_state, &displayed.disp);
    lua_setfield(game_state, -2, "char");
    lua_pushinteger(game_state, displayed.color_pair);
    lua_setfield(game_state, -2, "color");
}

void Tile::retrieve() {
    lua_getfield(game_state, -1, "char");
    displayed.disp = lua_tostring(game_state, -1)[0];
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "color");
    displayed.color_pair = lua_tointeger(game_state, -1);
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "passable");
    passable = lua_toboolean(game_state, -1);
    lua_pop(game_state, 2);
}

void Tile::addItem(Item* it) {
    for(auto i : items) {
        if(i->getName() == it->getName()) {
            if(i->getStack() + it->getStack() < 100) {
                i->setStack(i->getStack() + it->getStack());
                return;
            } else if(i->getStack() < 100) {
                uint16_t n = 99 - i->getStack();
                i->setStack(99);
                it->setStack(it->getStack() - n);
            }
        }
    }
    items.push_back(it);
}
