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

        if(statistics.xp >= statistics.max_xp)
            level_up();
        if(statistics.hp <= 0) {
            statistics.hp = 0;
            add_message(message("You succumb to your wounds", 8));
            add_message(message("And so, your quest comes to a tragic end..."));
            draw_messages();
            update_panels();
            doupdate();
            getch();
            exit(0);
        }
    }
    virtual void draw(WINDOW* window, uint16_t corner);
    void draw_messages(void);
    void add_message(message mess);
    bool ask_question(message mess);
    void attacked(combat_result res, Unit* other);
    virtual void level_up(void);

    virtual combat_result attack(Unit* other);
    virtual bool should_attack(Unit* other);
protected:
    WINDOW* ui_window;
    PANEL*  ui_panel;
    WINDOW* stats_window;
    PANEL*  stats_panel;
    std::vector<message> messages;
};
