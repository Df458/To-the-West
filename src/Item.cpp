#include "item.h"
#include "util.h"

using namespace rapidxml;

Item::Item(std::string file) {
    char* data = load_file(file.c_str());
    xml_document<> doc;
    doc.parse<0>(data);
    xml_node<>* node = doc.first_node("item");

    delete[] data;
}
