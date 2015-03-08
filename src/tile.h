#ifndef TILE_H
#define TILE_H
#include<vector>
#include <curses.h>
#include "util.h"
#include "item.h"

class Tile : public LuaObject {
public:
    void draw(WINDOW* win, int x, int y);
    inline uint8_t getCost() { return move_cost; }
    inline void setSymbol(symbol sym) { displayed = sym; }
protected:
    std::string enter_func;
    std::string leave_func;

    bool occupied = false;
    bool passable = false;
    symbol displayed;
    std::vector<Item*> items;
    uint8_t move_cost = 1;
};
#endif
