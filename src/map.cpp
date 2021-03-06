#include "map.h"
#include "player.h"
#include "effect.h"
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
        if(auto a = i->first_attribute("level")) {
            biomes[biomes.size() - 1].level = atoi(a->value());
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
    box(map_window, 0, 0);
    uint16_t corner = clamp((int)active_unit->getPosition().x - 35, 0, 922);
    for(uint16_t i = corner; i < corner + 78; ++i) {
        for(int j = 0; j < 18; ++j) {
            map_data[i][j]->draw(map_window, i - corner + 1, j + 1);
        }
    }

    drawUnits(corner);

    update_panels();
    doupdate();
    for (auto i : effect_list) {
        i->apply();
        i->draw(map_window, corner);
    }

    top_panel(map_panel);
    //update_panels();
    doupdate();
    wmove(map_window, active_unit->getPosition().y + 1, active_unit->getPosition().x - corner + 1);
}

void Map::drawUnits(uint16_t corner) {
    for(uint8_t j = 0; j < 4; ++j) {
        for(auto i : unit_list[j])
            i->draw(map_window, corner);
    }
}

void Map::update(uint16_t time) {
    if(player->getPosition().x <= 50 && !hasboss) {
        spawnAt(vec2(1, 9), "mochus");
        hasboss = true;
    }
    if(rand() % 10 < 1) {
        spawn();
    }

    for(uint8_t j = 0; j < 4; ++j) {
        for(uint16_t i = 0; i < unit_list[j].size(); ++i) {
            Unit* u = unit_list[j][i];
            u->update(time);
            if(!u->getAlive() || !u) {
                if(u == player) {
                    u->setTime(-1);
                    return;
                }
                if(u)
                    delete u;
                unit_list[j].erase(unit_list[j].begin() + i);
                --i;
                continue;
            }
        }
    }

    for(uint16_t i = 0; i < effect_list.size(); ++i) {
        Effect* e = effect_list[i];
        e->apply();
        if(e->canDelete()) {
            delete e;
            effect_list.erase(effect_list.begin() + i);
            --i;
        }
    }
}

void Map::spawn(void) {
    uint16_t corner = clamp((int)active_unit->getPosition().x - 35, 0, 922);
    int32_t x = 0;
    if((rand() % 3 != 0 || corner > 807) && corner > 35) {
        x = rand() % 25 - 50;
    } else {
        x = rand() % 25 + 105;
    }
    x = clamp((int32_t)corner + x, 0, 1000);
    uint16_t y = rand() % 18;
    if(!map_data[x][y]->getPassable() || map_data[x][y]->getOccupied() || !biomes[map_data[x][y]->getBiome()].get_unit(50))
        return;
    addUnit(new Unit(biomes[map_data[x][y]->getBiome()].get_unit((800 - x) / 50), vec2(x, y)));
}

void Map::spawnAt(vec2 position, std::string name) {
    position.x = clamp((int32_t)position.x, 0, 1000);
    position.y = clamp((int32_t)position.y, 0, 17);
    if(!map_data[position.x][position.y]->getPassable() || map_data[position.x][position.y]->getOccupied())
        return;
    addUnit(new Unit(name, position));
}

void Map::generate(void) {
    for(uint16_t i = 0; i < biomes.size(); ++i) {
        biomes[i].set_index(i);
    }

    uint16_t pos = 0;
    while(pos < 1000) {
        uint16_t b = 0;
        while(true) {
            uint16_t selection = rand() % (freq_max + 1);
            uint16_t mark = 0;
            for(uint16_t i = 0; i < biomes.size(); ++i) {
                if(biome_freqs[i] + mark >= selection) {
                    b = i;
                    break;
                } else
                    mark += biome_freqs[i];
            }
            if(1000 - biomes[b].level * 60 >= pos)
                break;
        }
        uint16_t end = clamp(pos + (biomes[b].get_size() / 2) + rand() % biomes[b].get_variance(), 0, 1000);
        for(uint16_t j = 0; j < 18; ++j) {
            uint16_t x = clamp(pos - rand() % (biomes[b].get_spread() / 2), 0, 1000);
            if(pos == 0)
                x = 0;
            for(uint16_t i = x; i < end && i < 1000; ++i) {
                uint16_t y = j;
                map_data[i][y] = biomes[b].get_tile(vec2(i, y));
            }
        }
        pos = end;
    }

    for(int i = 0; i < 25; ++i)
        for(int j = 0; j < 18; ++j) {
            map_data[i][j]->setSymbol(symbol('>', 0));
            map_data[i][j]->setPassable(true);
            map_data[i][j]->clearfuncs();
        }
    for(int j = 0; j < 18; ++j)
        map_data[0][j]->setSymbol(symbol('|', 0));
}

Unit* Map::getTarget(vec2 position, uint8_t faction) {
    vector<Unit*>* list;
    switch(faction) {
        case 0: {
            uint8_t r = rand() % 2 + 1;
            list = &unit_list[r];
            } break;
        case 1:
            list = &unit_list[0];
            break;
        case 2: {
            uint8_t r = rand() % 2 * 3;
            list = &unit_list[r];
            } break;
        case 3:
            list = &unit_list[2];
            break;
        default:
            return NULL;
    }

    if(!list)
        return NULL;

    for(uint32_t i = 0; i < list->size(); ++i) {
        if(abs(position.x - list->at(i)->getPosition().x) < 20) {
            return list->at(i);
        }
    }

    return NULL;
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

Unit* Biome::get_unit(int16_t lev) {
    uint16_t selection = rand() % (unit_freqs_max + 1);
    uint16_t mark = 0;
    for(uint16_t i = 0; i < units.size(); ++i) {
        if(unit_freqs[i] + mark >= selection) {
            Unit* u = new Unit(units[i]);
            for(int i = level; i < lev; ++i) {
                u->level_up();
            }
            return u;
        } else
            mark += unit_freqs[i];
    }
    return NULL;
}

Tile* Biome::get_tile(vec2 pos) {
    uint16_t selection = rand() % (freqs + 1);
    uint16_t mark = 0;
    for(uint16_t i = 0; i < terrains.size(); ++i) {
        if(terrain_freqs[i] + mark >= selection) {
            return new Tile(terrains[i], pos);
        } else
            mark += terrain_freqs[i];
    }
    return NULL;
}

void Biome::set_index(uint16_t index) {
    for(auto& i : terrains) {
        i.setBiome(index);
    }
}
