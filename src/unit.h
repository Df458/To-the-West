#ifndef UNIT_H
#define UNIT_H
#include "luaobj.h"
#include "util.h"
#include <panel.h>
#include <set>

// Factions:
// 0: Good guys - aggro on 1, 2
// 1: Bad guys  - aggro on 0
// 2: Monsters  - aggro on 0, 3
// 3: Animals   - aggro on 2

struct stats {
    float max_hp = 1;
    float hp = 1;

    uint16_t strength = 1;
    uint16_t accuracy = 1;
    uint16_t defense = 1;
    uint16_t dodge = 1;
    uint16_t speed = 1;

    uint16_t level = 1;
    uint16_t xp = 0;
    uint16_t max_xp = 25;
    uint16_t xp_value = 0;
};

class Unit : public LuaObject {
public:
    Unit(void);
    Unit(std::string file);
    Unit(Unit* copy, vec2 pos);
    virtual ~Unit() {}

    virtual void draw(WINDOW* window, uint16_t corner);
    const inline vec2& getPosition(void) const { return position; }
    virtual void created(void) { call(create_func); }
    virtual void insert(void);
    virtual void retrieve(void);
    inline std::string getName(void) const { return name; }
    virtual void update(uint16_t time);
    inline uint8_t getFaction(void) const { return faction; }
    bool move(vec2 delta);
    virtual combat_result attack(Unit* other);
    void die(void);
    inline bool getAlive(void) { return alive; }
    inline int16_t getTime() { return time; }
    inline void setTime(int16_t t) { time = t; }
    virtual bool should_attack(Unit* other);
    void level_up(void);
    void make_enemy(Unit* u) { if(enemies.find(u) == enemies.end()) enemies.insert(u); }
    void lose_enemy(Unit* u) { if(enemies.find(u) != enemies.end()) enemies.erase(u); }
    void hes_dead_jim() { lose_enemy(target); target = NULL; }

protected:
    Unit* target = NULL;
    std::set<Unit*> enemies;
    std::string name;
    vec2 position;
    stats statistics;
    uint8_t faction = 0;
    int16_t time = 0;
    bool hostile = true;
    bool controlled = false;
    bool alive = true;
    std::string create_func;
    std::string die_func;
    std::string move_func;
    std::string attack_func;
    std::string update_func;
    int16_t regen_timer;
    symbol displayed;
};
#endif
