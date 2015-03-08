#include "player.h"
#include "map.h"
#include "util.h"
#include <curses.h>

Player::Player(void) : Unit() {
    displayed.disp = '@';
    move_func = "test.lua";
}

int Player::get_input(void) {
    int time_passed = 0;
    char input = getch();
    switch(input) {
        case 'h':
            if(position.x > 0)
                --position.x;
            time_passed = 2 * map->tile_at(position)->getCost();
            call(move_func);
            break;
        case 'j':
            if(position.y < 17)
                ++position.y;
            time_passed = 2 * map->tile_at(position)->getCost();
            call(move_func);
            break;
        case 'k':
            if(position.y > 0)
                --position.y;
            time_passed = 2 * map->tile_at(position)->getCost();
            call(move_func);
            break;
        case 'l':
            if(position.x < 2999)
                ++position.x;
            time_passed = 2 * map->tile_at(position)->getCost();
            call(move_func);
            break;
        case 'y':
            if(position.x > 0 && position.y > 0) {
                --position.x;
                --position.y;
            }
            time_passed = 3 * map->tile_at(position)->getCost();
            call(move_func);
            break;
        case 'u':
            if(position.x < 2999 && position.y > 0) {
                ++position.x;
                --position.y;
            }
            time_passed = 3 * map->tile_at(position)->getCost();
            call(move_func);
            break;
        case 'b':
            if(position.x > 0 && position.y < 17) {
                --position.x;
                ++position.y;
            }
            time_passed = 3 * map->tile_at(position)->getCost();
            call(move_func);
            break;
        case 'n':
            if(position.x < 2999 && position.y < 17) {
                ++position.x;
                ++position.y;
            }
            time_passed = 3 * map->tile_at(position)->getCost();
            call(move_func);
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
