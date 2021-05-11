#include "Monster.h"

#include "Game.h"

Monster::Monster() {
    //ctor
}

Monster::Monster(float _speed, Coord *_startCoord) : Being(0.0f, 0.0f, _speed, _startCoord) {
    //ctor
    dead = false;
}

Monster::~Monster() {
    //dtor
}

void Monster::moveMonster(Game *game) {
    return;
}

//==============================

Monster1::Monster1() {
    //ctor
}

Monster1::Monster1(float _speed, Coord *_startCoord) : Monster(_speed, _startCoord) {
    //ctor
}

Monster1::~Monster1() {
    //dtor
}

void Monster1::moveMonster(Game *game) {

    game->currentLevel->levelMap->mapString[((int)beingCoord->y) * game->currentLevel->levelMap->getWidth() + (int)beingCoord->x] = ' ';

    while(!dead) {
        // Follow the player
        game->currentLevel->levelMap->mapString
        [((int)beingCoord->x) * game->currentLevel->levelMap->getWidth() + beingCoord->y] = ' ';
        if(beingCoord->x > (int)game->player->beingCoord->x) {
            beingCoord->x--;
        } else if(beingCoord->y > (int)game->player->beingCoord->y) {
            beingCoord->y--;
        } else if(beingCoord->x < (int)game->player->beingCoord->x) {
            beingCoord->x++;
        } else if(beingCoord->y < (int)game->player->beingCoord->y) {
            beingCoord->y++;
        }
        game->currentLevel->levelMap->mapString[((int)beingCoord->x) * game->currentLevel->levelMap->getWidth() + beingCoord->y] = 'e';

        if((int)game->player->beingCoord->x == (int)beingCoord->x && (int)game->player->beingCoord->y == (int)beingCoord->y)
            game->player->health--;

        if(!game->debug)
            PlaySound(TEXT("sounds/bruh.wav"), NULL, SND_ASYNC);

        this_thread::sleep_for(chrono::milliseconds((int)( 1000 * fSpeed)));
    }

    game->currentCaption->mapString   = L"           You stab the enemy in the heart                  ";
    game->currentCaption->mapString  += L"               It dies and drops a key                      ";

    game->currentLevel->items[0]->has = true;
    game->player->inv->mapString[19] = '1';
    return;
}

//==============================

Monster2::Monster2() {
    //ctor
}

Monster2::Monster2(float _speed, Coord *_startCoord) : Monster(_speed, _startCoord) {
    //ctor
}

Monster2::~Monster2() {
    //dtor
}

void Monster2::moveMonster(Game *game) {
    srand(time(NULL));
    int i = 1;

    // Just moves back and forth
    while(!dead) {
        game->currentLevel->levelMap->mapString[((int)beingCoord->x) * game->currentLevel->levelMap->getWidth() + (int)beingCoord->y] = ' ';
        beingCoord->y += i;
        game->currentLevel->levelMap->mapString[((int)beingCoord->x) * game->currentLevel->levelMap->getWidth() + beingCoord->y] = 'e';
        this_thread::sleep_for(chrono::seconds((int)(1 * fSpeed)));

        if((int)game->player->beingCoord->x == (int)beingCoord->x && (int)game->player->beingCoord->y == (int)beingCoord->y)
            game->player->health--;

        game->currentLevel->levelMap->mapString[((int)beingCoord->x) * game->currentLevel->levelMap->getWidth() + (int)beingCoord->y] = ' ';
        beingCoord->y += i;
        game->currentLevel->levelMap->mapString[((int)beingCoord->x) * game->currentLevel->levelMap->getWidth() + beingCoord->y] = 'e';
        this_thread::sleep_for(chrono::seconds((int)(1 * fSpeed)));

        if((int)game->player->beingCoord->x == (int)beingCoord->x && (int)game->player->beingCoord->y == (int)beingCoord->y)
            game->player->health--;


        //PlaySound(TEXT("sounds/bruh.wav"), NULL, SND_ASYNC);
        i *= -1;

    }
    game->currentLevel->levelMap->mapString[((int)beingCoord->x) * game->currentLevel->levelMap->getWidth() + (int)beingCoord->y] = ' ';

    return;
}
