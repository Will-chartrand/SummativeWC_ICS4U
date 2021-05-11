#include "Person.h"

#include "Game.h"

Person::Person() {
    //ctor
}

Person::Person(float angle, float FOV, float speed, Layout *_inv, Coord *_startCoord) : Being(angle, FOV, speed, _startCoord) {
    inv = _inv;
    inv->mapString  = L"sword:   0";
    inv->mapString += L"key:     0";
    inv->mapString += L"Potion:  0";
    inv->mapString += L"          ";
    inv->mapString += L"          ";

    health = 3;
}

Person::~Person() {
    //dtor
}

void Person::update(Game *game){
        char collidingWith = game->currentLevel->levelMap->mapString.c_str()[(int)game->player->beingCoord->x * game->currentLevel->levelMap->getWidth() + (int)game->player->beingCoord->y];
        // Handle CCW Rotation
        if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
            game->player->fPlayerA -= (game->player->fSpeed * 0.75f) * game->fElapsedTime;

        // Handle CW Rotation
        if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
            game->player->fPlayerA += (game->player->fSpeed * 0.75f) * game->fElapsedTime;

        // Handle Forwards movement & collision
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
            game->player->beingCoord->x += sinf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            game->player->beingCoord->y += cosf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;

            collidingWith = game->currentLevel->levelMap->mapString.c_str()[(int)game->player->beingCoord->x * game->currentLevel->levelMap->getWidth() + (int)game->player->beingCoord->y];
            if (!game->debug && (collidingWith == '#' || collidingWith == 'I')) {
                game->player->beingCoord->x -= sinf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
                game->player->beingCoord->y -= cosf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            }
        }

        // Handle backwards movement & collision
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
            game->player->beingCoord->x -= sinf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            game->player->beingCoord->y -= cosf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            collidingWith = game->currentLevel->levelMap->mapString.c_str()[(int)game->player->beingCoord->x * game->currentLevel->levelMap->getWidth() + (int)game->player->beingCoord->y];
            if (!game->debug && (collidingWith == '#' || collidingWith == 'I')) {
                game->player->beingCoord->x += sinf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
                game->player->beingCoord->y += cosf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
            game->player->beingCoord->y -= sinf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            game->player->beingCoord->x += cosf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            collidingWith = game->currentLevel->levelMap->mapString.c_str()[(int)game->player->beingCoord->x * game->currentLevel->levelMap->getWidth() + (int)beingCoord->y];
            if (!game->debug && (collidingWith == '#' || collidingWith == 'I')) {
                beingCoord->y += sinf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
                beingCoord->x -= cosf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
            game->player->beingCoord->y += sinf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            game->player->beingCoord->x -= cosf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            collidingWith = game->currentLevel->levelMap->mapString.c_str()[(int)game->player->beingCoord->x * game->currentLevel->levelMap->getWidth() + (int)game->player->beingCoord->y];
            if (!game->debug && (collidingWith == '#' || collidingWith == 'I')) {
                game->player->beingCoord->y -= sinf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
                game->player->beingCoord->x += cosf(game->player->fPlayerA) * game->player->fSpeed * game->fElapsedTime;
            }
        }

        if(GetAsyncKeyState((unsigned short)'X') & 0x8000)
            game->debug = true;

        //handle sprinting
        if (GetAsyncKeyState((unsigned short)VK_SHIFT) & 0x8000) {
            game->player->fSpeed = 5.0f;
        } else {
            game->player->fSpeed = 3.0f;
        }

        onCurrentTile = game->currentLevel->levelMap->mapString[((int)beingCoord->x) * game->currentLevel->levelMap->getWidth() + (int)beingCoord->y];

        if(health == 0) {
            dead = true;
            game->currentCaption->mapString  = L"                   Your health                              ";
            game->currentCaption->mapString += L"               is zero, you are dead                         ";
        }


        return;
}

void Person::SWING_DA_SWORD(Game *game) {
    while(!dead) {

        if(GetAsyncKeyState((unsigned short)VK_SPACE) & 0x8000) {
//            game->castOneRay(5.0f, fPlayerA, 167, true);
            game->castOneRay(2.0f, fPlayerA, 'e', true);
            game->strike = true;
            this_thread::sleep_for(chrono::milliseconds(300));
            game->strike = false;
        }

    }
    return;
}
