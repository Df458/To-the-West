#include "unit.h"
#include "map.h"
#include "player.h"
#include <cstring>
#include <rapidxml.hpp>

using namespace std;
using namespace rapidxml;

Unit::Unit(void) {
}

Unit::Unit(Unit* copy, vec2 pos) {
    if(!copy)
        return;
    name = copy->name;
    position = pos;
    statistics = copy->statistics;
    faction = copy->faction;
    hostile = copy->hostile;
    create_func = copy->create_func;
    die_func = copy->die_func;
    move_func = copy->move_func;
    attack_func = copy->attack_func;
    update_func = copy->update_func;
    displayed = copy->displayed;

    map->tile_at(position)->setOccupied(true);
    map->tile_at(position)->setOccupant(this);
    map->tile_at(position)->call(map->tile_at(position)->get_enter_func());
    call(create_func);
}

Unit::Unit(std::string file) {
    char* data = load_file(("/data/creatures/" + file + ".xml").c_str());
    xml_document<> doc;
    doc.parse<0>(data);
    auto node = doc.first_node("creature");
    if(auto a = node->first_attribute("name"))
        name = a->value();

    if(auto n = node->first_node("symbol")) {
        char disp = '@';
        uint16_t color = 0;
        if(auto a = n->first_attribute("char"))
            disp = a->value()[0];
        if(auto a = n->first_attribute("color"))
            color = atoi(a->value());
        displayed = symbol(disp, color);
    }

    if(auto n = node->first_node("stats")) {
        if(auto a = n->first_attribute("hp"))
            statistics.max_hp = atof(a->value());
        if(auto a = n->first_attribute("str"))
            statistics.strength = atoi(a->value());
        if(auto a = n->first_attribute("def"))
            statistics.defense = atoi(a->value());
        if(auto a = n->first_attribute("spd"))
            statistics.speed = atoi(a->value());
        if(auto a = n->first_attribute("dog"))
            statistics.dodge = atoi(a->value());
        if(auto a = n->first_attribute("acc"))
            statistics.accuracy = atoi(a->value());
        statistics.hp = statistics.max_hp;
    }
    
    if(auto n = node->first_node("ai")) {
        if(auto a = n->first_attribute("faction"))
            faction = atoi(a->value());
        if(auto a = n->first_attribute("hostile"))
            hostile = strcmp(a->value(), "false");
    }

    delete[] data;
}

void Unit::draw(WINDOW* window, uint16_t corner) {
    if(position.x < corner || position.x > corner + 77)
        return;

    wattron(window, COLOR_PAIR(displayed.color_pair));
    mvwaddch(window, position.y + 1, position.x - corner + 1, displayed.disp);
    wattroff(window, COLOR_PAIR(displayed.color_pair));
}

void Unit::update(uint16_t time) {
    if(!alive)
        return;

    call(update_func);

    if(!target) {
        //target = player;
    }
    else {
        vec2 dist = position - target->position;
        move(step(dist));
    }
}

void Unit::insert(void) {
    lua_pushinteger(game_state, position.x);
    lua_setfield(game_state, -2, "x");
    lua_pushinteger(game_state, position.y);
    lua_setfield(game_state, -2, "y");
}

bool Unit::move(vec2 delta) {
    Tile* prev = map->tile_at(position);
    Tile* next = map->tile_at(position + delta);
    if(next->getOccupied() || !next->getPassable() || (position.x == 0 && delta.x < 0) || position.x + delta.x > 2999 || (position.y == 0 && delta.y < 0) || position.y + delta.y > 17) {
        if(next->getOccupied() && should_attack(next->getOccupant())) {
            combat_result res = attack(next->getOccupant());
            if(next->getOccupant() == player)
                player->attacked(res, this);
        }
        return false;
    }

    prev->call(prev->get_leave_func());
    prev->setOccupied(false);
    prev->setOccupant(NULL);
    position += delta;
    next->call(next->get_enter_func());
    next->setOccupied(true);
    next->setOccupant(this);

    return true;
}

void Unit::die(void) {
    map->tile_at(position)->setOccupied(false);
    map->tile_at(position)->setOccupant(NULL);
    alive = false;
    call(die_func);
}

combat_result Unit::attack(Unit* other) {
    combat_result retval;
    if(!other)
        return retval;
    if(other != player)
        other->target = this;
    uint16_t hit_roll   = rand() % statistics.accuracy + clamp(statistics.accuracy / 2 - 5, 0, 25);
    uint16_t dodge_roll = rand() % other->statistics.dodge + clamp(other->statistics.dodge / 2 - 5, 0, 25);
    if(hit_roll < dodge_roll)
        return retval;
    retval.flags += 0b100;
    uint16_t block_roll = rand() % other->statistics.defense + clamp(other->statistics.defense / 2 - 3, 0, 25);
    float mod = 1.0f;
    if(hit_roll < block_roll) {
        mod = 0.5f;
        retval.flags += 0b010;
    }
    retval.damage = clamp((rand() % statistics.strength + clamp(statistics.strength / 2, 0, 25)) * mod - clamp(other->statistics.defense / 2 - 5, 0, 25), 1, 1000);
    other->statistics.hp -= retval.damage;
    if(other->statistics.hp <= 0) {
        other->die();
        retval.flags += 0b001;
    }
    return retval;
}

bool Unit::should_attack(Unit* other) {
    if(!other)
        return false;
    if(other->target == this || target == other)
        return true;
    switch(faction) {
        case 0:
            return other->faction == 1 || other->faction == 2;
        case 1:
            return other->faction == 0;
        case 2:
            return other->faction == 0 || other->faction == 3;
        case 3:
            return other->faction == 2;
    }
    return true;
}
