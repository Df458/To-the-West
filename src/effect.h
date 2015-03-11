#ifndef EFFECT_H
#define EFFECT_H
#include "util.h"
#include "luaobj.h"
#include <string>
#include <curses.h>

class Effect : public LuaObject {
public:
    Effect(bool v, symbol c, std::string ont = "", std::string onu = "") : visible(v), character(c), ontile_callback(ont), onunit_callback(onu) {}
    Effect(vec2 position, float radius, bool v, symbol c, std::string ont = "", std::string onu = "");
    Effect(vec2 tile, bool v, symbol c, std::string ont = "", std::string onu = "");
    Effect(vec2 from, vec2 to, bool pierce, bool v, symbol c, std::string ont = "", std::string onu = "");
    virtual ~Effect() {}
    inline bool canDelete(void) { return counter == 2; }

    void apply();
    void draw(WINDOW* win, uint16_t corner);
protected:
    bool visible = true;
    uint8_t counter = 0;
    symbol character;
    std::string ontile_callback;
    std::string onunit_callback;
    std::vector<vec2> affected_list;
};

#endif
