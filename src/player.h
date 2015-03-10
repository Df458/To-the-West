#include "unit.h"

class Player : public Unit {
public:
    Player(void);
    int get_input(void);
    void update(uint16_t t) final {
        time = 0;
        regen_timer -= t;
        if(regen_timer <= 0) {
            regen_timer = 20;
            statistics.hp++;
        }
        if(statistics.hp > statistics.max_hp)
            statistics.hp = statistics.max_hp;
    }
    virtual void draw(WINDOW* window, uint16_t corner);
    void draw_messages(void);
    void add_message(message mess);
    void attacked(combat_result res, Unit* other);

    virtual combat_result attack(Unit* other);
    virtual bool should_attack(Unit* other);
protected:
    WINDOW* ui_window;
    PANEL*  ui_panel;
    std::vector<message> messages;
};
