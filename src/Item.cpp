#include "item.h"
#include "util.h"
#include <cstring>

using namespace rapidxml;

Item::Item(std::string file) {
    char* data = load_file(file.c_str());
    xml_document<> doc;
    doc.parse<0>(data);
    xml_node<>* node = doc.first_node("item");

    if(auto a = node->first_attribute("name"))
        name = a->value();

    for(auto n = node->first_node("script"); n; n = n->next_sibling("script")) {
        if(auto a = n->first_attribute("type")) {
            if(!strcmp(a->value(), "use")) {
                if(auto i = n->first_attribute("id"))
                    use_func = i->value();
            }
        }
        if(auto a = n->first_attribute("type")) {
            if(!strcmp(a->value(), "use")) {
                if(auto i = n->first_attribute("id"))
                    use_func = i->value();
            }
        }
    }

    delete[] data;
}
