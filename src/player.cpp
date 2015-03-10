#include "player.h"
#include "map.h"
#include <cmath>
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
    stats_window = newwin(20, 40, 0, 40);
    box(stats_window, 0, 0);

    statistics.max_hp = 25;
    statistics.hp = 25;
    statistics.accuracy = 17;
    statistics.strength = 18;
    statistics.defense = 16;
    statistics.dodge = 14;
    statistics.speed = 15;
}

int Player::get_input(void) {
    time = 0;
    char input = getch();
    switch(input) {
        case 'h':
            move(vec2(-1, 0));
            //time_passed = 2 * map->tile_at(position)->getCost();
            break;
        case 'j':
            move(vec2(0, 1));
            //time_passed = 2 * map->tile_at(position)->getCost();
            break;
        case 'k':
            move(vec2(0, -1));
            //time_passed = 2 * map->tile_at(position)->getCost();
            break;
        case 'l':
            move(vec2(1, 0));
            //time_passed = 2 * map->tile_at(position)->getCost();
            break;
        case 'y':
            move(vec2(-1, -1));
            //time_passed = 3 * map->tile_at(position)->getCost();
            break;
        case 'u':
            move(vec2(1, -1));
            //time_passed = 3 * map->tile_at(position)->getCost();
            break;
        case 'b':
            move(vec2(-1, 1));
            //time_passed = 3 * map->tile_at(position)->getCost();
            break;
        case 'n':
            move(vec2(1, 1));
            //time_passed = 3 * map->tile_at(position)->getCost();
            break;
        case 's':
            stats_panel = new_panel(stats_window);
            update_panels();
            doupdate();
            getch();
            del_panel(stats_panel);
            update_panels();
            doupdate();
            break;
        case 'q':
            add_message(message("Really quit? (y/n)", 4));
            draw_messages();
            update_panels();
            doupdate();
            while(char c = getch()) {
                if(c == 'y')
                    return -1;
                if(c == 'n')
                    break;
            }
        break;
        case '?':
            display_help();
            return 0;
            break;
    }

    return -time;
}

void Player::draw(WINDOW* window, uint16_t corner) {
    Unit::draw(window, corner);

    uint16_t hp_color = 10;
    if(statistics.hp / statistics.max_hp < .5f && statistics.hp / statistics.max_hp >= .25f)
        hp_color = 11;
    else if(statistics.hp / statistics.max_hp < .25f)
        hp_color = 8;
    mvwprintw(window, 19, 1, "HP: ");
    wattron(window, COLOR_PAIR(hp_color));
    mvwprintw(window, 19, 5, (to_string((uint16_t)statistics.hp) + "/" + to_string((uint16_t)statistics.max_hp)).c_str());
    wattroff(window, COLOR_PAIR(hp_color));

    wclear(stats_window);
    box(stats_window, 0, 0);
    mvwprintw(stats_window, 2, 1, ("Level:     " + to_string(statistics.level)).c_str());
    mvwprintw(stats_window, 4, 1, ("Health:    " + to_string((int)statistics.max_hp)).c_str());
    mvwprintw(stats_window, 6, 1, ("Strength:  " + to_string(statistics.strength)).c_str());
    mvwprintw(stats_window, 8, 1, ("Defense:   " + to_string(statistics.defense)).c_str());
    mvwprintw(stats_window, 10, 1, ("Dodge:     " + to_string(statistics.dodge)).c_str());
    mvwprintw(stats_window, 12, 1, ("Speed:     " + to_string(statistics.speed)).c_str());
    mvwprintw(stats_window, 14, 1, ("XP:        " + to_string(statistics.xp) + "/" + to_string(statistics.max_xp)).c_str());

    draw_messages();
}

void Player::draw_messages() {
    wclear(ui_window);
    box(ui_window, 0, 0);

    uint16_t j = 0;
    for(int32_t i = messages.size() - 1; i >= 0 && j < 9; --i) {
        wattron(ui_window, COLOR_PAIR(messages[i].color));
        mvwprintw(ui_window, 9 - j, 1, messages[i].text.c_str());
        wattroff(ui_window, COLOR_PAIR(messages[i].color));
        ++j;
    }
}

void Player::add_message(message mess) {
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
    //draw_messages();
    //update_panels();
    //doupdate();
}

combat_result Player::attack(Unit* other) {
    combat_result res = Unit::attack(other);
    if(!res.flags)
        add_message(("You miss the " + other->getName() + "."));
    else if(res.flags & 0b001)
        add_message(message("You slay the " + other->getName() + "!"));
    else if(res.flags & 0b010)
        add_message(message("Your blow glances off the " + other->getName() + " for " + to_string(res.damage) + " damage."));
    else
        add_message(message("You hit the " + other->getName() + " for " + to_string(res.damage) + " damage."));
    return res;
}

void Player::attacked(combat_result res, Unit* other) {
    if(!res.flags)
        add_message(("The " + other->getName() + " misses."));
    else if(res.flags & 0b001 || alive == false) {
        statistics.hp = 0;
        add_message(message("The " + other->getName() + " strikes a killing blow!", 8));
        add_message(message("And so, your quest comes to a tragic end..."));
        draw_messages();
        update_panels();
        doupdate();
        getch();
        exit(0);
    } else if(res.flags & 0b010)
        add_message(message("The " + other->getName() +  "'s blow glances off your armor for " + to_string(res.damage) + " damage.", 11));
    else
        add_message(message("The " + other->getName() + " hits you for " + to_string(res.damage) + " damage.", 11));
}

bool Player::should_attack(Unit* other) {
    if(!other)
        return false;
    if(!Unit::should_attack(other)) {
        add_message(message("Really attack the " + other->getName() + "? (y/n)", 4));
        draw_messages();
        update_panels();
        doupdate();
        while(char c = getch()) {
            if(c == 'y')
                return true;
            if(c == 'n')
                return false;
        }
        return false;
    }
    return true;
}

void Player::level_up(void) {
    wclear(stats_window);
    box(stats_window, 0, 0);
    mvwprintw(stats_window, 2, 1, ("Level:     " + to_string(statistics.level)).c_str());
    mvwprintw(stats_window, 4, 1, ("Health:    " + to_string((int)statistics.max_hp)).c_str());
    mvwprintw(stats_window, 6, 1, ("Strength:  " + to_string(statistics.strength)).c_str());
    mvwprintw(stats_window, 8, 1, ("Defense:   " + to_string(statistics.defense)).c_str());
    mvwprintw(stats_window, 10, 1, ("Dodge:     " + to_string(statistics.dodge)).c_str());
    mvwprintw(stats_window, 12, 1, ("Speed:     " + to_string(statistics.speed)).c_str());
    statistics.xp = 0;
    statistics.level++;

    statistics.hp += 5;
    statistics.max_hp += 5;
    statistics.strength += 2;
    statistics.defense += 2;
    statistics.dodge += 2;
    statistics.speed += 2;

    statistics.max_xp = pow(10, (float)statistics.level / 3) * statistics.level + 15;

    wattron(stats_window, A_BLINK | COLOR_PAIR(6));
    mvwprintw(stats_window, 1, 15, "Level Up!");
    wattroff(stats_window, A_BLINK);
    mvwprintw(stats_window, 2, 15, (">> " + to_string(statistics.level)).c_str());
    mvwprintw(stats_window, 4, 15, (">> " + to_string((int)statistics.max_hp)).c_str());
    mvwprintw(stats_window, 6, 15, (">> " + to_string(statistics.strength)).c_str());
    mvwprintw(stats_window, 8, 15, (">> " + to_string(statistics.defense)).c_str());
    mvwprintw(stats_window, 10, 15, (">> " + to_string(statistics.dodge)).c_str());
    mvwprintw(stats_window, 12, 15, (">> " + to_string(statistics.speed)).c_str());
    wattroff(stats_window, COLOR_PAIR(6));
    mvwprintw(stats_window, 14, 1, ("XP:        " + to_string(statistics.xp) + "/" + to_string(statistics.max_xp)).c_str());
    
    stats_panel = new_panel(stats_window);
    update_panels();
    doupdate();
    getch();
    del_panel(stats_panel);
    update_panels();
    doupdate();
}
