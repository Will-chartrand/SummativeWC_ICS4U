#include "Item.h"

Item::Item() {
    //ctor
}

Item::Item(char _character, Coord *_location) {
    character = _character;
    location = _location;
    has = false;
}

Item::~Item() {
    //dtor
}

void Item::effect(Game *game) {
    return;
}
