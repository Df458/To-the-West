#include "item.h"
#include "util.h"
#include <cstring>

using namespace rapidxml;

Item::Item(std::string file) {
    char* data = load_file(("/data/items/" + file + ".xml").c_str());
    xml_document<> doc;
    doc.parse<0>(data);
    xml_node<>* node = doc.first_node("item");

    if(auto a = node->first_attribute("name"))
        name = a->value();

    if(auto a = node->first_attribute("equipable"))
        equipable = strcmp(a->value(), "false");

    for(auto n = node->first_node("script"); n; n = n->next_sibling("script")) {
        if(auto a = n->first_attribute("type")) {
            if(!strcmp(a->value(), "use")) {
                if(auto i = n->first_attribute("id"))
                    use_func = i->value();
            } else if(!strcmp(a->value(), "throw-unit")) {
                if(auto i = n->first_attribute("id"))
                    throw_unit_func = i->value();
            } else if(!strcmp(a->value(), "throw-tile")) {
                if(auto i = n->first_attribute("id"))
                    throw_tile_func = i->value();
            } else if(!strcmp(a->value(), "step")) {
                if(auto i = n->first_attribute("id"))
                    step_func = i->value();
            } else if(!strcmp(a->value(), "take")) {
                if(auto i = n->first_attribute("id"))
                    take_func = i->value();
            } else if(!strcmp(a->value(), "equip")) {
                if(auto i = n->first_attribute("id"))
                    equip_func = i->value();
            } else if(!strcmp(a->value(), "unequip")) {
                if(auto i = n->first_attribute("id"))
                    unequip_func = i->value();
            } else if(!strcmp(a->value(), "drop")) {
                if(auto i = n->first_attribute("id"))
                    drop_func = i->value();
            }
        }
    }

    delete[] data;
}
