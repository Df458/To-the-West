#ifndef TILE_H
#define TILE_H
#include<vector>
#include <curses.h>
#include "util.h"
#include "item.h"

class Tile : public LuaObject {
public:
    Tile() {}
    Tile(symbol s, int cost) : displayed(s), move_cost(cost) {}
    void draw(WINDOW* win, int x, int y);
    inline uint8_t getCost() { return move_cost; }
    inline void setSymbol(symbol sym) { displayed = sym; }
    inline void setBiome(uint16_t index) { biome_index = index; }
    inline uint16_t getBiome(void) { return biome_index; }
protected:
    std::string enter_func;
    std::string leave_func;

    bool occupied = false;
    bool passable = false;
    symbol displayed;
    std::vector<Item*> items;
    uint8_t move_cost = 1;
    uint16_t biome_index = 0;
};
#endif
