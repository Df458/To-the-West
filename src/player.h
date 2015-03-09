#include "unit.h"

class Player : public Unit {
public:
    Player(void);
    int get_input(void);
    void update(uint16_t time) final {}
    virtual void draw(WINDOW* window, uint16_t corner);
    void add_message(message mess);
protected:
    WINDOW* ui_window;
    PANEL*  ui_panel;
    std::vector<message> messages;
};
