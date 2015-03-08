#include "tile.h"
#include <curses.h>

void Tile::draw(WINDOW* win, int x, int y) {
    wattron(win, COLOR_PAIR(displayed.color_pair));
    mvwaddch(win, y, x, displayed.disp);
    wattroff(win, COLOR_PAIR(displayed.color_pair));
}
