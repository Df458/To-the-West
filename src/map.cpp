#include "map.h"
#include <string>
#include "unit.h"

using namespace std;

Map::Map(Unit* player_unit) {
    active_unit = player_unit;
    addUnit(player_unit);
    map_window = newwin(20, 80, 0, 0);
    box(map_window, 0, 0);
    map_panel = new_panel(map_window);
}

void Map::addUnit(Unit* unit) {
    unit_list.push_back(unit);
    unit->created();
}

void Map::draw(void) {
    uint16_t corner = clamp((int)active_unit->getPosition().x - 35, 0, 2999);
    for(uint16_t i = corner; i < corner + 78; ++i) {
        for(int j = 0; j < 18; ++j) {
            map_data[i][j].draw(map_window, i - corner + 1, j + 1);
        }
    }

    for(auto i : unit_list)
        i->draw(map_window, corner);
    top_panel(map_panel);
    update_panels();
    doupdate();
    wmove(map_window, active_unit->getPosition().y + 1, active_unit->getPosition().x - corner + 1);
}

void Map::update(void) {
}
