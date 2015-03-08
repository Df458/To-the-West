#include "map.h"
#include <string>
#include <rapidxml.hpp>
#include "unit.h"

using namespace std;
using namespace rapidxml;

Map::Map(Unit* player_unit) {
    active_unit = player_unit;
    addUnit(player_unit);
    map_window = newwin(20, 80, 0, 0);
    box(map_window, 0, 0);
    map_panel = new_panel(map_window);
}

void Map::addUnit(Unit* unit) {
    unit_list.push_back(unit);
    unit->created();
}

void Map::draw(void) {
    uint16_t corner = clamp((int)active_unit->getPosition().x - 35, 0, 2999);
    for(uint16_t i = corner; i < corner + 78; ++i) {
        for(int j = 0; j < 18; ++j) {
            map_data[i][j].draw(map_window, i - corner + 1, j + 1);
        }
    }

    for(auto i : unit_list)
        i->draw(map_window, corner);
    top_panel(map_panel);
    update_panels();
    doupdate();
    wmove(map_window, active_unit->getPosition().y + 1, active_unit->getPosition().x - corner + 1);
}

void Map::update(void) {
}

void Map::generate(void) {
    biomes.push_back(Biome("woods"));
    biomes.push_back(Biome("plains"));
    biomes.push_back(Biome("desert"));
    uint16_t pos = 0;
    while(pos < 2999) {
        uint16_t end = clamp(pos + 100 + rand() % 100, 0, 2999);
        uint16_t b = rand() % (biomes.size());
        for(uint16_t j = 0; j < 18; ++j) {
            uint16_t x = clamp(pos - rand() % 12, 0, 2999);
            if(pos == 0)
                x = 0;
            for(uint16_t i = x; i < end && i < 3000; ++i) {
                uint16_t y = j;
                map_data[i][y] = biomes[b].get_tile();
            }
        }
        pos = end;
    }
}

Biome::Biome(std::string file) {
    char* data = load_file(("/data/biomes/" + file + ".xml").c_str());
    xml_document<> doc;
    doc.parse<0>(data);
    auto node = doc.first_node("biome");
    for(auto i = node->first_node("tile", 4, false); i; i = i->next_sibling("tile", 4, false)) {
        uint16_t freq = 1;
        char sym = ' ';
        uint16_t color = 0;
        uint8_t cost = 1;

        if(auto a = i->first_attribute("symbol"))
            sym = a->value()[0];
        if(auto a = i->first_attribute("color"))
            color = atoi(a->value());
        if(auto a = i->first_attribute("cost"))
            cost = atoi(a->value());
        if(auto a = i->first_attribute("freq"))
            freq = atoi(a->value());

        symbol s(sym, color);
        terrains.push_back(Tile(s, cost));
        terrain_freqs.push_back(freq);
        freqs += freq;
    }
    delete[] data;
    name = file;
}

Tile Biome::get_tile(void) {
    uint16_t selection = rand() % (freqs + 1);
    uint16_t mark = 0;
    for(uint16_t i = 0; i < terrains.size(); ++i) {
        if(terrain_freqs[i] + mark >= selection)
            return terrains[i];
        else
            mark += terrain_freqs[i];
    }
    return Tile();
}
