#ifndef TILE_H
#define TILE_H
#include<vector>
#include <curses.h>
#include "util.h"
#include "item.h"

class Tile : public LuaObject {
public:
    Tile() {}
    Tile(symbol s, int cost, std::string en = "", std::string le = "") : displayed(s), move_cost(cost), enter_func(en), leave_func(le) {}
    void draw(WINDOW* win, int x, int y);
    inline uint8_t getCost() { return move_cost; }
    inline void setSymbol(symbol sym) { displayed = sym; }
    inline void setBiome(uint16_t index) { biome_index = index; }
    inline uint16_t getBiome(void) { return biome_index; }

    inline std::string get_enter_func(void) { return enter_func; }
    inline std::string get_leave_func(void) { return leave_func; }
protected:
    bool occupied = false;
    bool passable = false;
    symbol displayed;
    std::vector<Item*> items;
    uint8_t move_cost = 1;
    uint16_t biome_index = 0;
    std::string enter_func;
    std::string leave_func;
};
#endif
