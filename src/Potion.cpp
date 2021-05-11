#include "Potion.h"
#include "Game.h"

Potion::Potion() {
    //ctor
}

Potion::Potion(char _character, Coord *_location) : Item(_character, _location){
    //ctor
}

Potion::~Potion() {
    //dtor
}

void Potion::effect(Game *game) {

}

//==========================

Potion1::Potion1(char _character, Coord *_location) : Potion(_character, _location) {
    //ctor
}

Potion1::~Potion1() {
    //dtor
}

void Potion1::effect(Game *game) {
    game->player->fFOV = (sin(float(game->FOVmultiplier++)/70.0f) + 2);
    return;
}



//==========================

Potion2::Potion2(char _character, Coord *_location) : Potion(_character, _location) {
    //ctor
}

Potion2::~Potion2() {
    //dtor
}

void Potion2::effect(Game *game) {
    game->player->health = 3;
    return;
}



//==========================
