#ifndef UNIT_H
#define UNIT_H
#include "luaobj.h"
#include "util.h"
#include <panel.h>

class Unit : public LuaObject {
public:
    Unit(void);

    virtual void draw(WINDOW* window, uint16_t corner);
    const inline vec2& getPosition(void) const { return position; }
    virtual void created(void) { call(create_func); }
    virtual void insert(void);

protected:
    vec2 position;
    std::string create_func;
    std::string die_func;
    std::string move_func;
    std::string attack_func;
    std::string update_func;
    symbol displayed;
};
#endif
