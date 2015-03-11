#include <curses.h>
#include <string>
#include "luafunc.h"
#include "map.h"
#include "player.h"
#include "util.h"
#include "luaobj.h"
using namespace std;

lua_State* game_state;
uint16_t color_flags;
Player* player;
Map* map;

uint8_t COLOR_LIGHTGREEN = 10;
uint8_t COLOR_LIGHTBLUE  = 12;
uint8_t COLOR_BROWN = 17;
uint8_t COLOR_GRAY = 8;

bool start_menu() {
    WINDOW* menu_window = newwin(20, 80, 0, 0);
    PANEL* menu_panel = new_panel(menu_window);
    top_panel(menu_panel);

    int position = 3;
    box(menu_window, 0, 0);
    while(true) {
        mvwprintw(menu_window, 1, 1, "-To the West-");
        mvwprintw(menu_window, 3, 1, "Start a new game");
        mvwprintw(menu_window, 5, 1, "Load a saved game");
        mvwprintw(menu_window, 7, 1, "Help");
        mvwprintw(menu_window, 9, 1, "Exit");
        update_panels();
        doupdate();
        move(position, 1);
        char input = getch();
        switch(input) {
            case 'j':
            case '2':
                if(position < 9)
                    position += 2;
                break;
            case 'k':
            case '8':
                if(position > 3)
                    position -= 2;
                break;
            case 'q':
                del_panel(menu_panel);
                delwin(menu_window);
                return false;
            case ' ':
            case '\n':
                switch(position) {
                    case 3:
                        del_panel(menu_panel);
                        delwin(menu_window);
                        return true;
                    case 5:
                        error("Loading games is not implemented");
                        del_panel(menu_panel);
                        delwin(menu_window);
                        return true;
                        break;
                    case 7:
                        display_help();
                        top_panel(menu_panel);
                        refresh();
                        break;
                    case 9:
                        del_panel(menu_panel);
                        delwin(menu_window);
                        return false;
                }
        }
    }
    del_panel(menu_panel);
    delwin(menu_window);
    
    return true;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    game_state = luaL_newstate();
    luaL_openlibs(game_state);
    luaL_openlib(game_state, "game", lua_global_functions, 0);
    
    //lua_newtable(game_state);

    initscr();
    clear();
    noecho();
    cbreak();

    color_flags = NO_COLOR;
    start_color();
    /* Special color pairs
     * 0 - White on black, default
     * 1 - White on red,   error
    */
    if(has_colors()) {
        if(COLORS == 8) {
            COLOR_LIGHTGREEN = COLOR_GREEN;
            COLOR_LIGHTBLUE = COLOR_BLUE;
            COLOR_GRAY = COLOR_WHITE;
        }
        color_flags += BASIC_COLORS;
        init_pair(1, COLOR_RED, COLOR_WHITE); // Error color
        init_pair(2, COLOR_LIGHTGREEN,  COLOR_BLACK); // Green on black
        init_pair(4, COLOR_YELLOW,      COLOR_BLACK); // Yellow on black
        init_pair(3, COLOR_YELLOW,      COLOR_BLACK); // Brown on black
        init_pair(5, COLOR_GREEN,       COLOR_BLACK); // Dark Green on black
        init_pair(6, COLOR_LIGHTBLUE,   COLOR_BLACK);
        init_pair(7, COLOR_BLUE,        COLOR_BLACK);
        init_pair(8, COLOR_RED,         COLOR_BLACK);
        init_pair(10, COLOR_LIGHTGREEN, COLOR_BLACK); // Green on black
        init_pair(11, COLOR_YELLOW,     COLOR_BLACK); // Yellow on black
        init_pair(12, COLOR_YELLOW,     COLOR_BLACK); // Brown on black
        init_pair(13, COLOR_BLACK,      COLOR_WHITE); // White on black
        init_pair(14, COLOR_BLACK,      COLOR_RED); // White on black
        init_pair(15, COLOR_GRAY,         COLOR_BLACK);
        if(can_change_color()) {
            color_flags += CAN_CHANGE_COLOR;
            if(COLORS >= 256 && COLOR_PAIRS >=256) {
                color_flags += MANY_COLORS;
            }
            init_color(COLOR_RED, 975, 80, 0);
            init_color(COLOR_WHITE, 900, 900, 900);
            init_color(COLOR_BROWN, 600, 300, 50);
            init_pair(3, COLOR_BROWN, COLOR_BLACK); // Brown on black
            init_pair(12, COLOR_BLACK, COLOR_BROWN); // Brown on black
        }
    }

    player = new Player();
    map = new Map(player);

    int time_passed = 0;

    if(!start_menu())
        return 0;

    map->generate();
    map->draw();
    update_panels();
    doupdate();
    while(time_passed >= 0) {
        time_passed = player->get_input();
        map->update(time_passed);
        map->draw();
        update_panels();
        doupdate();
    }

    endwin();
    return 0;
}
