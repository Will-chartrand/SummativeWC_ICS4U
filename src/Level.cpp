#include "Level.h"

#include "Game.h"

Level::Level() {
    //ctor
}

Level::Level(Coord *_spawnCoord, Coord *_exitCoord, float _spawnA, Layout *_levelMap, Item *_items[]) {
    spawnCoord = _spawnCoord;
    exitCoord = _exitCoord;
    spawnA = _spawnA;
    levelMap = _levelMap;

    for(int i = 0; i < 5; i++) {
        items[i] = _items[i];
    }
}

Level::~Level() {
    //dtor
}

void Level::update(Game *_game) {
    // Set player position in new level
    _game->player->beingCoord->x = _game->currentLevel->spawnCoord->x;
    _game->player->beingCoord->y = _game->currentLevel->spawnCoord->y;
    _game->player->prevBeingCoord->x = _game->player->beingCoord->x;
    _game->player->prevBeingCoord->y = _game->player->beingCoord->y;
    _game->player->fPlayerA = _game->currentLevel->spawnA;

    // Reset fog of war
    _game->fogOfWar->mapString  = L"###############";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    _game->fogOfWar->mapString += L"###############";


    // Reset move stacks
    _game->autoMove->deleteStack();
    _game->backTrackStack->deleteStack();
    _game->n = new Node(0, nullptr);
    _game->n2 = new Node(0, nullptr);
    _game->autoMove = new Stack(_game->n, _game->n);
    _game->backTrackStack = new Stack(_game->n2, _game->n2);
    _game->autoMove->push(0);
    _game->backTrackStack->push(_game->autoMove->pop());      // push the backtracking move
    _game->autoMove->push(4);
    _game->autoMove->push(3);
    _game->autoMove->push(2);
    _game->autoMove->push(1);
    _game->autoMove->push(0);

    return;
}

//=============================

Level1::Level1() {
    //ctor
}

Level1::Level1(Coord *_spawnCoord, Coord *_exitCoord, float _spawnA, Layout *_levelMap, Item *_items[]) : Level(_spawnCoord, _exitCoord, _spawnA, _levelMap, _items) {
    //ctor
}

Level1::~Level1() {
    //dtor
}

void Level1::update(Game *_game) {

    if(items[3]->has) {
        items[3]->effect(_game);
    }

    //inventory detection for first map
    if(_game->player->onCurrentTile == 'k') {
        _game->player->inv->mapString[19] = '1';
        items[0]->has = true;
        levelMap->mapString[((int)_game->player->beingCoord->x) * levelMap->getHeight() + (int)_game->player->beingCoord->y] = ' ';
        _game->currentCaption->mapString  = L"                      A key.                                ";
        _game->currentCaption->mapString += L"      Maybe it could be used for something later            ";

    }

    if(_game->player->onCurrentTile == 'n' && !_game->note1->has) {
        _game->note1->has = true;
        SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
        system("start notes/note1.txt");
        SetConsoleActiveScreenBuffer(_game->hConsole);
    }

    if(items[0]->has && _game->castOneRay(1.0f, _game->player->fPlayerA, 'I', false)) {
        _game->player->inv->mapString[19] = '0';
        levelMap->mapString[/*(int)items[1]->location->x */ 13 * levelMap->getWidth() +  11/*(int)items[1]->location->y*/] = ' ';
        _game->currentCaption->mapString  = L"                        wowza!                              ";
        _game->currentCaption->mapString += L"                  The door unlocks!                         ";
    }

    if(_game->player->onCurrentTile == items[3]->character) {
        _game->player->inv->mapString[29] = '1';
        levelMap->mapString[((int)_game->player->beingCoord->x) * levelMap->getWidth() + (int)_game->player->beingCoord->y] = ' ';
        items[3]->has = true;
        _game->currentCaption->mapString  = L"                  You Feel                                  ";
        _game->currentCaption->mapString += L"                   D r Un K                                 ";
    }

    if(int(_game->player->beingCoord->x) == int(_game->currentLevel->exitCoord->x) && int(_game->player->beingCoord->y) == int(_game->currentLevel->exitCoord->y)) {
        _game->currentCaption->mapString  = L"            You hear other footsteps closeby                ";
        _game->currentCaption->mapString += L"     maybe they could be blocking the way to something      ";

        _game->currentLevel = _game->lvl2;
        _game->currentLevel->Level::update(_game);

        if(!_game->autoSolve) {
            std::thread doTheMonsterMash2(&Monster2::moveMonster, _game->mnstr2, _game); // Do the monster mash
            doTheMonsterMash2.detach();
        }

        _game->player->fFOV = 3.14159f / 3.0f;
    }
    return;
}

//=============================

Level2::Level2() {
    //ctor
}

Level2::Level2(Coord *_spawnCoord, Coord *_exitCoord, float _spawnA, Layout *_levelMap, Item *_items[]) : Level(_spawnCoord, _exitCoord, _spawnA, _levelMap, _items) {
    //ctor
}

Level2::~Level2() {
    //dtor
}

void Level2::update(Game *_game) {

    if(_game->player->onCurrentTile == 'n' && !_game->note2->has) {
        _game->note2->has = true;
        SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
        system("cd notes && start note2.bat");
        SetConsoleActiveScreenBuffer(_game->hConsole);

        _game->currentCaption->mapString = L"                   A goose lurks closeby                     ";
        _game->currentCaption->mapString += L"            (beat next level to get rid of him)             ";
    }

    if(_game->player->onCurrentTile == 's') {
        _game->player->inv->mapString[9] = '1';
        items[1]->has = true;
        _game->currentCaption->mapString = L"          You may need this in the near future               ";
        _game->currentCaption->mapString += L"            Press SPACE to swing your sword                 ";
        levelMap->mapString[((int)_game->player->beingCoord->x) * levelMap->getWidth() + (int)_game->player->beingCoord->y] = ' ';

        std::thread attackThread(&Person::SWING_DA_SWORD, _game->player, _game);
        attackThread.detach();

    }

    if(_game->player->onCurrentTile == '@') {
        if(!_game->autoSolve) {
            std::thread doTheMonsterMash(&Monster1::moveMonster, _game->mnstr1, _game);
            doTheMonsterMash.detach();
        }
        _game->currentLevel = _game->lvl3;
        _game->currentLevel->Level::update(_game);

        _game->currentCaption->mapString  = L"                   An enemy approaches.                        ";
        _game->currentCaption->mapString += L"         You hear keys clinking together as it walks           ";

    }
    return;
}

//=============================

Level3::Level3() {
    //ctor
}

Level3::Level3(Coord *_spawnCoord, Coord *_exitCoord, float _spawnA, Layout *_levelMap, Item *_items[]) : Level(_spawnCoord, _exitCoord, _spawnA, _levelMap, _items) {
    //ctor
}

Level3::~Level3() {
    //dtor
}

void Level3::update(Game *_game) {
    _game->mnstr2->dead = true;
    if((items[0]->has || _game->autoSolve) && _game->castOneRay(1.0f, _game->player->fPlayerA, 'I', false)) {
        levelMap->mapString[(int)items[1]->location->x * levelMap->getWidth() + (int)items[1]->location->y] = ' ';
        _game->currentCaption->mapString  = L"                        wowie!                              ";
        _game->currentCaption->mapString += L"                  The door Opens !!                         ";
    }

    if(_game->player->onCurrentTile == '@') {
        _game->currentCaption->mapString  = L"           Get to the end to beat the game                  ";
        _game->currentCaption->mapString += L"                                                            ";

        _game->currentLevel = _game->lvl4;

        _game->mnstr1->dead = true;

        _game->player->beingCoord->x = _game->currentLevel->spawnCoord->x;
        _game->player->beingCoord->y = _game->currentLevel->spawnCoord->y;
        _game->player->fPlayerA = _game->currentLevel->spawnA;
        //reset fog of war
        _game->currentLevel->Level::update(_game);

        if(_game->note2->has) {
            SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
            system("cd notes/DesktopGoose && start CloseGoose.bat");
            SetConsoleActiveScreenBuffer(_game->hConsole);
        }

        if(!_game->autoSolve) {
            std::thread doTheMonsterMash3(&Monster2::moveMonster, _game->mnstr3, _game);
            doTheMonsterMash3.detach();

            std::thread doTheMonsterMash4(&Monster2::moveMonster, _game->mnstr4, _game);
            doTheMonsterMash4.detach();
        }
    }

    return;
}

//=============================

Level4::Level4() {
    //ctor
}

Level4::Level4(Coord *_spawnCoord, Coord *_exitCoord, float _spawnA, Layout *_levelMap, Item *_items[]) : Level(_spawnCoord, _exitCoord, _spawnA, _levelMap, _items) {
    //ctor
}

Level4::~Level4() {
    //dtor
}

void Level4::update(Game *_game) {
    if(_game->player->onCurrentTile == items[0]->character) {
        _game->player->inv->mapString[29] = '1';
        levelMap->mapString[((int)_game->player->beingCoord->x) * levelMap->getWidth() + (int)_game->player->beingCoord->y] = ' ';
        items[0]->has = true;

        items[0]->effect(_game);


        _game->currentCaption->mapString  = L"                  Your health                               ";
        _game->currentCaption->mapString += L"               has been restored                            ";
    }

     if(_game->player->onCurrentTile == '@') {

        _game->mnstr3->dead = true;
        _game->mnstr4->dead = true;

            SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
            system("start notes/goodbye.txt");
            SetConsoleActiveScreenBuffer(_game->hConsole);
            _game->quit = true;

            _game->player->dead = true;
    }
    return;
}


