#ifndef MAP_H
#define MAP_H
#include "tile.h"
#include <panel.h>
#include <vector>

class Unit;

class Map {
public:
    Map(Unit*);

    void addUnit(Unit* unit);
    void draw(void);
    void update(void);
    inline Tile* tile_at(vec2 position) { return &map_data[position.x][position.y]; }

protected:
    Unit* active_unit;
    Tile map_data[3000][18];
    std::vector<Unit*> unit_list;
    WINDOW* map_window;
    PANEL* map_panel;
};
#endif
