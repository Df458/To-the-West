#include "unit.h"

Unit::Unit(void) {
}

void Unit::draw(WINDOW* window, uint16_t corner) {
    if(position.x < corner || position.y > corner + 78)
        return;

    attron(COLOR_PAIR(displayed.color_pair));
    mvwaddch(window, position.y + 1, position.x - corner + 1, displayed.disp);
    attroff(COLOR_PAIR(displayed.color_pair));
}

void Unit::insert(void) {
    lua_pushinteger(game_state, position.x);
    lua_setfield(game_state, -2, "x");
    lua_pushinteger(game_state, position.y);
    lua_setfield(game_state, -2, "y");
}
