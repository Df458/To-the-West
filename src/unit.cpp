#include "unit.h"
#include "map.h"
#include "player.h"
#include <cstring>
#include <cmath>
#include <rapidxml.hpp>

using namespace std;
using namespace rapidxml;

Unit::Unit(void) {
    regen_timer = 20;
}

Unit::Unit(Unit* copy, vec2 pos) {
    if(!copy)
        return;
    regen_timer = 20;
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
        if(auto a = n->first_attribute("value"))
            statistics.xp_value = atoi(a->value());
        statistics.hp = statistics.max_hp;
    }
    
    if(auto n = node->first_node("ai")) {
        if(auto a = n->first_attribute("faction"))
            faction = atoi(a->value());
        if(auto a = n->first_attribute("hostile"))
            hostile = strcmp(a->value(), "false");
    }

    for(auto n = node->first_node("script"); n; n = n->next_sibling("script")) {
        if(auto at = n->first_attribute("type")) {
            if(!strcmp(at->value(), "update"))
                update_func = n->first_attribute("id")->value();
        }
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

void Unit::update(uint16_t t) {
    if(!alive)
        return;

    regen_timer -= t;
    if(regen_timer <= 0) {
        regen_timer = 20;
        statistics.hp++;
    }
    if(statistics.hp > statistics.max_hp)
        statistics.hp = statistics.max_hp;

    time += t;

    if(statistics.xp >= statistics.max_xp)
        level_up();

    if(target && target->getAlive()) {
        lua_newtable(game_state);
        target->insert();
        lua_setglobal(game_state, "target");
    }

    call(update_func);

    if(target) {
        lua_getglobal(game_state, "target");
        target->retrieve();
    }

    while(time > 0) {
        if(!target && hostile) {
            if((faction == 1 || faction == 2) && abs(position.x - player->position.x) < 40) {
                target = player;
                target->make_enemy(this);
            } else if((target = map->getTarget(position, faction))) {
                target->make_enemy(this);
            } else {
                move(vec2(rand() % 3 - 1, rand() % 3 - 1));
            }
        } else if(target){
            vec2 dist = position - target->position;
            if(abs(dist.x) > 50 || !target->getAlive()) {
                target = NULL;
                time = 0;
            } else
                move(step(dist));
        } else
            move(vec2(rand() % 3 - 1, rand() % 3 - 1));
    }
}

void Unit::insert(void) {
    lua_pushinteger(game_state, position.x);
    lua_setfield(game_state, -2, "x");
    lua_pushinteger(game_state, position.y);
    lua_setfield(game_state, -2, "y");

    lua_pushnumber(game_state, statistics.hp);
    lua_setfield(game_state, -2, "hp");
    lua_pushnumber(game_state, statistics.max_hp);
    lua_setfield(game_state, -2, "hp_max");
    lua_pushinteger(game_state, statistics.strength);
    lua_setfield(game_state, -2, "str");
    lua_pushinteger(game_state, statistics.accuracy);
    lua_setfield(game_state, -2, "acc");
    lua_pushinteger(game_state, statistics.defense);
    lua_setfield(game_state, -2, "def");
    lua_pushinteger(game_state, statistics.dodge);
    lua_setfield(game_state, -2, "dog");
    lua_pushinteger(game_state, statistics.speed);
    lua_setfield(game_state, -2, "spd");

    lua_pushboolean(game_state, target == player);
    lua_setfield(game_state, -2, "targeting_player");
    lua_pushstring(game_state, name.c_str());
    lua_setfield(game_state, -2, "name");
    lua_pushinteger(game_state, faction);
    lua_setfield(game_state, -2, "faction");
    lua_pushinteger(game_state, time);
    lua_setfield(game_state, -2, "time");
    lua_pushboolean(game_state, alive);
    lua_setfield(game_state, -2, "alive");
    lua_pushboolean(game_state, hostile);
    lua_setfield(game_state, -2, "hostile");
    lua_pushboolean(game_state, controlled);
    lua_setfield(game_state, -2, "controlled");
}

void Unit::retrieve(void) {
    lua_getfield(game_state, -1, "hp");
    statistics.hp = lua_tonumber(game_state, -1);
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "hp_max");
    statistics.max_hp = lua_tonumber(game_state, -1);
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "str");
    statistics.strength = lua_tointeger(game_state, -1);
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "acc");
    statistics.accuracy = lua_tointeger(game_state, -1);
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "def");
    statistics.defense = lua_tointeger(game_state, -1);
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "dog");
    statistics.dodge = lua_tointeger(game_state, -1);
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "spd");
    statistics.speed = lua_tointeger(game_state, -1);
    lua_pop(game_state, 1);
    lua_getfield(game_state, -1, "time");
    time = lua_tointeger(game_state, -1);
    lua_pop(game_state, 2);
}

bool Unit::move(vec2 delta) {
    Tile* prev = map->tile_at(position);
    Tile* next = map->tile_at(position + delta);
    if(next->getOccupied() || !next->getPassable() || (position.x == 0 && delta.x < 0) || position.x + delta.x > 2999 || (position.y == 0 && delta.y < 0) || position.y + delta.y > 17) {
        if(next->getOccupied() && should_attack(next->getOccupant())) {
            Unit* hit = next->getOccupant();
            combat_result res = attack(hit);
            if(hit == player)
                player->attacked(res, this);
        } else {
            time = 0;
        }
        return false;
    }


    lua_pushboolean(game_state, true);
    lua_setglobal(game_state, "keep_moving");
    call(prev->get_leave_func());
    lua_getglobal(game_state, "keep_moving");
    if(!lua_toboolean(game_state, -1)) {
        lua_pop(game_state, 1);
        return false;
    }
    lua_pop(game_state, 1);
    prev->setOccupied(false);
    prev->setOccupant(NULL);
    position += delta;
    lua_pushboolean(game_state, true);
    lua_setglobal(game_state, "keep_moving");
    call(next->get_enter_func());
    lua_getglobal(game_state, "keep_moving");
    if(!lua_toboolean(game_state, -1)) {
        prev->setOccupied(true);
        prev->setOccupant(this);
        position -= delta;
        lua_pop(game_state, 1);
        return false;
    }
    next->setOccupied(true);
    next->setOccupant(this);

    if(abs(delta.x) + abs(delta.y) == 2)
        time -= 3 * next->getCost();
    else
        time -= 2 * next->getCost();;

    return true;
}

void Unit::die(void) {
    map->tile_at(position)->setOccupied(false);
    map->tile_at(position)->setOccupant(NULL);
    for(auto u : enemies)
        u->hes_dead_jim();
    alive = false;
    call(die_func);
}

combat_result Unit::attack(Unit* other) {
    time -= 5;
    combat_result retval;
    if(!other)
        return retval;
    if(other != player) {
        other->target = this;
        make_enemy(other);
    }
    uint16_t hit_roll   = rand() % statistics.accuracy + clamp(statistics.accuracy / 2 - 5, 0, 25);
    uint16_t dodge_roll = rand() % other->statistics.dodge + clamp(other->statistics.dodge / 2 - 5, 0, 25);
    if(hit_roll < dodge_roll) {
        time -= 2;
        return retval;
    }
    retval.flags += 0b100;
    uint16_t block_roll = rand() % other->statistics.defense + clamp(other->statistics.defense / 2 - 3, 0, 25);
    float mod = 1.0f;
    if(hit_roll < block_roll) {
        time--;
        mod = 0.5f;
        retval.flags += 0b010;
    }
    retval.damage = clamp((rand() % statistics.strength + clamp(statistics.strength / 2, 0, 25)) * mod - clamp(other->statistics.defense / 2 - 5, 0, 25), 1, 1000);
    other->statistics.hp -= retval.damage;
    if(other->statistics.hp <= 0) {
        other->die();
        statistics.xp += other->statistics.xp_value;
        retval.flags += 0b001;
    }
    return retval;
}

bool Unit::should_attack(Unit* other) {
    if(!other)
        return false;
    if(enemies.find(target) != enemies.end() || target == other)
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

void Unit::level_up(void) {
    statistics.xp = 0;
    statistics.level++;

    statistics.hp += 5;
    statistics.max_hp += 5;
    statistics.strength += 2;
    statistics.defense += 2;
    statistics.dodge += 2;
    statistics.speed += 2;

    statistics.max_xp = pow(10, (float)(statistics.level < 3 ? 3 : statistics.level) / 3) * statistics.level+ 15;
}
