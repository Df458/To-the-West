#include "effect.h"
#include "map.h"
#include "player.h"
#include "unit.h"
#include <cmath>
#include <string>

Effect::Effect(vec2 tile, float radius, bool v, symbol c, std::string ont, std::string onu) : Effect(v, c, ont, onu) {
    float r = pow(radius, 2);
    for(int i = tile.x - radius; i <= tile.x + radius; ++i) {
        for(int j = tile.y - radius; j <= tile.y + radius; ++j) {
            if(i < 0 || i > 2998 || j < 0 || j > 17)
                continue;
            vec2 dist = tile - vec2(i, j);
            if(pow((float)dist.x, 2) + pow((float)dist.y, 2) == r);
                affected_list.push_back(vec2(i, j));
        }
    }
}

Effect::Effect(vec2 tile, bool v, symbol c, std::string ont, std::string onu) : Effect(v, c, ont, onu) {
    affected_list.push_back(tile);
}

Effect::Effect(vec2 from, vec2 to, bool pierce, bool v, symbol c, std::string ont, std::string onu) : Effect(v, c, ont, onu) {
    vec2 dist = to - from;
    if(dist.x != 0) {
        float angle = (float)dist.y / (float)dist.x;
        int j = from.y;
        float partial = 0;
        for(int i = from.x; i != to.x; i += dist.x / abs(dist.x)) {
            if(i != from.x || j != from.y) {
                affected_list.push_back(vec2(i, j));
                if((!map->tile_at(vec2(i, j))->getPassable() || map->tile_at(vec2(i, j))->getOccupied()) && !pierce)
                    return;
            }
            partial += angle;
            while(abs(partial) >= 0.5) {
                partial -= partial / abs(partial);
                j += dist.y / abs(dist.y);
                if(i != from.x || j != from.y) {
                    affected_list.push_back(vec2(i, j));
                    if((!map->tile_at(vec2(i, j))->getPassable() || map->tile_at(vec2(i, j))->getOccupied()) && !pierce)
                        return;
                }
            }
        }
    } else if(dist.y != 0){
        for(int i = from.y; i != to.y; i += dist.y / abs(dist.y)) {
            if(i != from.y) {
                affected_list.push_back(vec2(from.x, i));
                if((!map->tile_at(vec2(from.x, i))->getPassable() || map->tile_at(vec2(from.x, i))->getOccupied()) && !pierce)
                    return;
            }
        }
    }
    affected_list.push_back(to);
}

void Effect::apply() {
    if(counter == 0)
        counter = 1;
    else
        return;

    for(auto i : affected_list) {
        if(i.x < 0 || i.x > 2998 || i.y < 0 || i.y > 17)
            continue;
        Tile* t = map->tile_at(i);
        if(!ontile_callback.empty())
            t->call(ontile_callback);
        if(t->getOccupied() && !onunit_callback.empty())
            t->getOccupant()->call(onunit_callback);
    }
}

void Effect::draw(WINDOW* win, uint16_t corner) {
    if(counter == 1)
        counter = 2;
    if(!visible)
        return;
    for(auto i : affected_list) {
        if(i.x < 0 || i.x > 2998 || i.y < 0 || i.y > 17)
            continue;
        if(i.x < corner || i.x >= corner + 78)
            break;
        wattron(win, COLOR_PAIR(character.color_pair));
        mvwaddch(win, i.y + 1, i.x - corner + 1, character.disp);
        wattroff(win, COLOR_PAIR(character.color_pair));
    }
}
