#ifndef MAP_H
#define MAP_H
#include "tile.h"
#include <panel.h>
#include <vector>

class Unit;

class Biome {
public:
    Biome(std::string file);
    Tile get_tile(void);
protected:
    std::string name;
    uint16_t freqs = 0;
    std::vector<Tile> terrains;
    std::vector<uint16_t> terrain_freqs;
};

class Map {
public:
    Map(Unit*);

    void addUnit(Unit* unit);
    void draw(void);
    void update(void);
    void generate(void);
    inline Tile* tile_at(vec2 position) { return &map_data[position.x][position.y]; }

protected:
    Unit* active_unit;
    Tile map_data[3000][18];
    std::vector<Unit*> unit_list;
    WINDOW* map_window;
    PANEL* map_panel;
    std::vector<Biome> biomes;
};
#endif
