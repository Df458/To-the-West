#ifndef ITEM_H
#define ITEM_H
#include<string>
#include <rapidxml.hpp>
#include "luaobj.h"
#include "util.h"

class Item : public LuaObject {
public:
    Item(std::string file);
    Item(std::string file, uint16_t s) : Item(file) { stack = s; }

    std::string getName() { return name; }
    uint16_t getStack() { return stack; }
    void setStack(uint16_t s) { stack = s; }
protected:
    symbol displayed;

    uint16_t stack = 1;
    std::string name;
    bool equipable = false;

    std::string take_func;
    std::string use_func;
    std::string equip_func;
    std::string unequip_func;
    std::string step_func;
    std::string throw_tile_func;
    std::string throw_unit_func;
    std::string drop_func;
};
#endif
