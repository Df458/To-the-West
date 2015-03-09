#include "unit.h"
#include <cstring>
#include <rapidxml.hpp>

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
    // Update here
}

void Unit::insert(void) {
    lua_pushinteger(game_state, position.x);
    lua_setfield(game_state, -2, "x");
    lua_pushinteger(game_state, position.y);
    lua_setfield(game_state, -2, "y");
}
