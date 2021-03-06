#ifndef UTIL_H
#define UTIL_H
#include <cstdint>
#include <string>
#include <vector>

class Player;
class Map;

const uint8_t NO_COLOR = 0b000;
const uint8_t BASIC_COLORS = 0b100;
const uint8_t CAN_CHANGE_COLOR = 0b010;
const uint8_t MANY_COLORS = 0b001; // >=256 colors

extern uint16_t color_flags;
extern Map* map;
extern Player* player;

struct message {
    message(std::string ntext, uint16_t ncolor = 0) : text(ntext), color(ncolor) {}
    std::string text;
    uint16_t color;
};

struct vec2 {
    vec2(int32_t nx = 0, int32_t ny = 0) : x(nx), y(ny) {}
    int32_t x = 0;
    int32_t y = 0;
    vec2 operator+(vec2 rval) { return vec2(x + rval.x, y + rval.y); }
    vec2 operator-(vec2 rval) { return vec2(x - rval.x, y - rval.y); }
    void operator+=(vec2 rval) { x += rval.x; y += rval.y; }
    void operator-=(vec2 rval) { x -= rval.x; y -= rval.y; }
};

struct symbol {
    symbol(char c = '.', uint16_t u = 0) : disp(c), color_pair(u) {}
    char disp = '.';
    uint16_t color_pair = 0;
};

struct combat_result {
    int16_t  flags = 0;
    int16_t damage = 0;
};

std::string get_path(void);

void _error(const char* file, unsigned line, std::string message);
#define error(message) _error(__FILE__, __LINE__, message)

void display_help(void);

vec2 step(vec2 delta);
vec2 select_target();

const char* const help_lines[] = {
    "Introduction:",
    "The foul warlock Mochus has stolen the Band of the Stars,",
    "bringing it to his fortress deep in the western mountains.",
    "Journey to the western lands,",
    "find the Band,",
    "and slay this dark sorceror once and for all!",
    "",
    "Note from the creator:",
    "If a tile is underlined, it has items.",
    "If a tile has a colored background, it's probably an enemy.",
    "I didn't have time to indicate this better in-game.",
    "Sorry!",
    "",
    "Movement Controls:",
    "y k u 7 8 9",
    " \\|/   \\|/",
    "h-.-l 4-5-6  Movement keys",
    " /|\\   /|\\",
    "b j n 1 2 3",
    "",
    "Pressing . will cause the player to wait a short while.",
    "You may need to press it a few times to see any changes, however.",
    "This game is not quite turn-based, but rather uses a time-based system",
    "to determine when things act. Waiting passes the shortest time possible,",
    "but you can also press > to wait 10x longer.",
    "",
    "Action Controls:",
    "s: view stats",
    "e: examine",
    ",: take items",
    "i: view inventory",
    "a: apply(use) item",
    "t: throw item",
    "d: drop item",
    "",
    "Other Controls:",
    "",
    "<space>/<enter>: select",
    "?: view controls",
    "q: back/exit",
    NULL
};

#define clamp(x, lo, hi) ((x > hi) ? hi : ((x < lo) ? lo : x))

char* load_file(const char* filename);

#endif
