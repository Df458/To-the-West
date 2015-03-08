#ifndef ITEM_H
#define ITEM_H
#include<string>
#include "luaobj.h"
#include "util.h"

class Item : public LuaObject {
public:
protected:
    symbol displayed;
    std::string take_func;
    std::string use_func;
    std::string equip_func;
    std::string unequip_func;
    std::string step_func;
    std::string throw_func;
    std::string drop_func;
};
#endif
