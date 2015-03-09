#ifndef MAP_H
#define MAP_H
#include "tile.h"
#include <panel.h>
#include <vector>

class Unit;

class Biome {
public:
    Biome(std::string file);
    Tile* get_tile(void);
    Unit* get_unit(void);
    void set_index(uint16_t index);
    inline const uint16_t& get_size(void) const { return size; }
    inline const uint16_t& get_spread(void) const { return spread; }
    inline const uint16_t& get_variance(void) const { return size_variance; }
    inline std::string get_name(void) const { return name; }
protected:
    std::string name;
    uint16_t freqs = 0;
    uint16_t unit_freqs_max = 0;
    uint16_t spread = 10;
    uint16_t size = 100;
    uint16_t size_variance = 50;
    std::vector<Tile> terrains;
    std::vector<uint16_t> terrain_freqs;
    std::vector<Unit> units;
    std::vector<uint16_t> unit_freqs;
};

class Map {
public:
    Map(Unit*);

    void addUnit(Unit* unit);
    void draw(void);
    void update(uint16_t time);
    void generate(void);
    void spawn(void);
    inline Tile* tile_at(vec2 position) { return map_data[position.x][position.y]; }

protected:
    Unit* active_unit;
    Tile* map_data[3000][18];
    std::vector<Unit*> unit_list[4];
    WINDOW* map_window;
    PANEL* map_panel;
    std::vector<Biome> biomes;
    std::vector<uint16_t> biome_freqs;
    uint32_t freq_max = 0;
};
#endif
