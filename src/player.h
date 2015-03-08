#include "unit.h"

class Player : public Unit {
public:
    Player(void);
    int get_input(void);
    void update(uint16_t time) final {}
};
