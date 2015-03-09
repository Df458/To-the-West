#include "player.h"
#include "map.h"
#include "util.h"
#include <curses.h>
#include <string>

using namespace std;

Player::Player(void) : Unit() {
    displayed.disp = '@';
    move_func = "test.lua";
    position.x = 2979;
    position.y = 9;
    faction = 0;
    controlled = true;

    ui_window = newwin(11, 80, 20, 0);
    ui_panel = new_panel(ui_window);

    statistics.max_hp = 12;
    statistics.hp = 12;
    statistics.accuracy = 10;
    statistics.strength = 12;
    statistics.defense = 12;
    statistics.dodge = 12;
    statistics.speed = 12;
}

int Player::get_input(void) {
    int time_passed = 0;
    char input = getch();
    switch(input) {
        case 'h':
            move(vec2(-1, 0));
            time_passed = 2 * map->tile_at(position)->getCost();
            break;
        case 'j':
            move(vec2(0, 1));
            time_passed = 2 * map->tile_at(position)->getCost();
            break;
        case 'k':
            move(vec2(0, -1));
            time_passed = 2 * map->tile_at(position)->getCost();
            break;
        case 'l':
            move(vec2(1, 0));
            time_passed = 2 * map->tile_at(position)->getCost();
            break;
        case 'y':
            move(vec2(-1, -1));
            time_passed = 3 * map->tile_at(position)->getCost();
            break;
        case 'u':
            move(vec2(1, -1));
            time_passed = 3 * map->tile_at(position)->getCost();
            break;
        case 'b':
            move(vec2(-1, 1));
            time_passed = 3 * map->tile_at(position)->getCost();
            break;
        case 'n':
            move(vec2(1, 1));
            time_passed = 3 * map->tile_at(position)->getCost();
            break;
        case 'q':
            return -1;
        break;
        case '?':
            display_help();
            return 0;
            break;
    }

    return time_passed;
}

void Player::draw(WINDOW* window, uint16_t corner) {
    Unit::draw(window, corner);

    uint16_t j = 0;
    wclear(ui_window);
    box(ui_window, 0, 0);

    uint16_t hp_color = 2;
    if(statistics.hp / statistics.max_hp < .5f)
        hp_color = 4;
    else if(statistics.hp / statistics.max_hp < .25f)
        hp_color = 8;
    mvwprintw(window, 19, 1, "HP: ");
    wattron(window, COLOR_PAIR(hp_color));
    mvwprintw(window, 19, 5, (to_string((uint16_t)statistics.hp) + "/" + to_string((uint16_t)statistics.max_hp)).c_str());
    wattroff(window, COLOR_PAIR(hp_color));
    for(int32_t i = messages.size() - 1; i >= 0 && j < 9; --i) {
        wattron(ui_window, COLOR_PAIR(messages[i].color));
        mvwprintw(ui_window, 9 - j, 1, messages[i].text.c_str());
        wattroff(ui_window, COLOR_PAIR(messages[i].color));
        ++j;
    }
}

void Player::add_message(message mess) {
    // Need to do line breaks here
    while(mess.text.size() > 78) {
        uint16_t i = 78;
        uint offset = 0;
        for(i = 78; i >= 0; --i)
            if(mess.text[i] == ' ')
                break;
        if(i == 0)
            i = 78;
        else
            offset = 1;
        messages.push_back(message(mess.text.substr(0, i), mess.color));
        mess.text = mess.text.substr(i + offset, mess.text.size() - i);
    }

    messages.push_back(mess);
}

uint8_t Player::attack(Unit* other) {
    uint8_t res = Unit::attack(other);
    if(!res)
        add_message(("You miss the " + other->getName() + "."));
    else if(res & 0b001)
        add_message(message("You slay the " + other->getName() + "!"));
    else if(res & 0b010)
        add_message(message("Your blow glances off the " + other->getName() + "."));
    else
        add_message(message("You hit the " + other->getName() + "."));
    return res;
}
