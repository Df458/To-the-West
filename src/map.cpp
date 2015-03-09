#include "map.h"
#include <cstring>
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

    char* data = load_file("/data/mapdata.xml");
    xml_document<> doc;
    doc.parse<0>(data);
    auto node = doc.first_node("map");
    for(auto i = node->first_node("biome"); i; i = i->next_sibling("biome")) {
        uint16_t freq = 1;
        if(auto a = i->first_attribute("freq")) {
            freq = atoi(a->value());
        }
        if(auto a = i->first_attribute("type")) {
            biomes.push_back(Biome(a->value()));
            biome_freqs.push_back(freq);
            freq_max += freq;
        }
    }
    delete[] data;
}

void Map::addUnit(Unit* unit) {
    if(!unit)
        return;
    unit_list[unit->getFaction()].push_back(unit);
    unit->created();
}

void Map::draw(void) {
    uint16_t corner = clamp((int)active_unit->getPosition().x - 35, 0, 2922);
    for(uint16_t i = corner; i < corner + 78; ++i) {
        for(int j = 0; j < 18; ++j) {
            map_data[i][j].draw(map_window, i - corner + 1, j + 1);
        }
    }

    for(uint8_t j = 0; j < 4; ++j) {
        for(auto i : unit_list[j])
            i->draw(map_window, corner);
    }
    top_panel(map_panel);
    update_panels();
    doupdate();
    wmove(map_window, active_unit->getPosition().y + 1, active_unit->getPosition().x - corner + 1);
}

void Map::update(uint16_t time) {
    if(rand() % 10 < 1) {
        spawn();
    }

    for(uint8_t j = 0; j < 4; ++j) {
        for(auto i : unit_list[j])
            i->update(time);
    }
}

void Map::spawn(void) {
    uint16_t corner = clamp((int)active_unit->getPosition().x - 35, 0, 2922);
    int32_t x = 0;
    if((rand() % 3 != 0 || corner > 2807) && corner > 35) {
        x = rand() % 25 - 50;
    } else {
        x = rand() % 25 + 25;
    }
    x = clamp((int32_t)corner + x, 0, 3000);
    uint16_t y = rand() % 18;
    if(!map_data[x][y].getPassable() || map_data[x][y].getOccupied() || !biomes[map_data[x][y].getBiome()].get_unit())
        return;
    addUnit(new Unit(biomes[map_data[x][y].getBiome()].get_unit(), vec2(x, y)));
}

void Map::generate(void) {
    biomes.push_back(Biome("woods"));
    biomes.push_back(Biome("plains"));
    biomes.push_back(Biome("desert"));
    biomes.push_back(Biome("river"));
    
    for(uint16_t i = 0; i < biomes.size(); ++i) {
        biomes[i].set_index(i);
    }

    uint16_t pos = 0;
    while(pos < 3000) {
        uint16_t selection = rand() % (freq_max + 1);
        uint16_t mark = 0;
        uint16_t b = 0;
        for(uint16_t i = 0; i < biomes.size(); ++i) {
            if(biome_freqs[i] + mark >= selection) {
                b = i;
                break;
            } else
                mark += biome_freqs[i];
        }
        uint16_t end = clamp(pos + biomes[b].get_size() + rand() % biomes[b].get_variance(), 0, 3000);
        for(uint16_t j = 0; j < 18; ++j) {
            uint16_t x = clamp(pos - rand() % biomes[b].get_spread(), 0, 3000);
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
    if(auto a = node->first_attribute("size"))
        size = atoi(a->value());
    if(auto a = node->first_attribute("spread"))
        spread = atoi(a->value());
    if(auto a = node->first_attribute("vary"))
        size_variance = atoi(a->value());
    for(auto i = node->first_node("tile", 4, false); i; i = i->next_sibling("tile", 4, false)) {
        uint16_t freq = 1;
        char sym = ' ';
        uint16_t color = 0;
        uint8_t cost = 1;
        std::string enter = "";
        std::string leave = "";
        bool pass = true;

        if(auto a = i->first_attribute("symbol"))
            sym = a->value()[0];
        if(auto a = i->first_attribute("color"))
            color = atoi(a->value());
        if(auto a = i->first_attribute("cost"))
            cost = atoi(a->value());
        if(auto a = i->first_attribute("freq"))
            freq = atoi(a->value());
        if(auto a = i->first_attribute("enter"))
            enter = a->value();
        if(auto a = i->first_attribute("leave"))
            leave = a->value();
        if(auto a = i->first_attribute("passable"))
            pass = strcmp(a->value(), "false");

        symbol s(sym, color);
        terrains.push_back(Tile(s, cost, pass, enter, leave));
        terrain_freqs.push_back(freq);
        freqs += freq;
    }

    for(auto i = node->first_node("creature", 8, false); i; i = i->next_sibling("creature", 8, false)) {
        uint16_t freq = 1;
        string type;

        if(auto a = i->first_attribute("type"))
            type = a->value();
        if(auto a = i->first_attribute("freq"))
            freq = atoi(a->value());

        units.push_back(Unit(type));
        unit_freqs.push_back(freq);
        unit_freqs_max += freq;
    }
    delete[] data;
    name = file;
}

Unit* Biome::get_unit(void) {
    uint16_t selection = rand() % (unit_freqs_max + 1);
    uint16_t mark = 0;
    for(uint16_t i = 0; i < units.size(); ++i) {
        if(unit_freqs[i] + mark >= selection)
            return &units[i];
        else
            mark += unit_freqs[i];
    }
    return NULL;
}

Tile Biome::get_tile(void) {
    uint16_t selection = rand() % (freqs + 1);
    uint16_t mark = 0;
    for(uint16_t i = 0; i < terrains.size(); ++i) {
        if(terrain_freqs[i] + mark >= selection) {
            return terrains[i];
        } else
            mark += terrain_freqs[i];
    }
    return Tile();
}

void Biome::set_index(uint16_t index) {
    for(auto& i : terrains) {
        i.setBiome(index);
    }
}
