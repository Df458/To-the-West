#ifndef UTIL_H
#define UTIL_H
#include <cstdint>
#include <string>

class Player;
class Map;

const uint8_t NO_COLOR = 0b000;
const uint8_t BASIC_COLORS = 0b100;
const uint8_t CAN_CHANGE_COLOR = 0b010;
const uint8_t MANY_COLORS = 0b001; // >=256 colors

extern uint16_t color_flags;
extern Map* map;
extern Player* player;

struct vec2 {
    vec2(uint32_t nx = 0, uint32_t ny = 0) : x(nx), y(ny) {}
    uint32_t x = 0;
    uint32_t y = 0;
};

struct symbol {
    symbol(char c = '.', uint16_t u = 0) : disp(c), color_pair(u) {}
    char disp = '.';
    uint16_t color_pair = 0;
};

std::string get_path(void);

void _error(const char* file, unsigned line, std::string message);
#define error(message) _error(__FILE__, __LINE__, message)

void display_help(void);

const char* const help_lines[] = {
    "Controls:",
    "~=-_None_-=~",
    NULL
};

#define clamp(x, lo, hi) ((x > hi) ? hi : ((x < lo) ? lo : x))

char* load_file(const char* filename);

#endif
