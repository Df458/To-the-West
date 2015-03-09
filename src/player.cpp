#include "player.h"
#include "map.h"
#include "util.h"
#include <curses.h>

Player::Player(void) : Unit() {
    displayed.disp = '@';
    move_func = "test.lua";
    position.x = 2979;
    position.y = 9;
    faction = 0;
    controlled = true;

    ui_window = newwin(11, 80, 21, 0);
    ui_panel = new_panel(ui_window);
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
