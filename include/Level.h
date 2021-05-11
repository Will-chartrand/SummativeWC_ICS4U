#ifndef LEVEL_H
#define LEVEL_H

#include "Layout.h"
#include "Coord.h"
#include "Item.h"
#include "Monster.h"

class Game;

using namespace std;

class Level {
  public:
    Level();
    Level(Coord*, Coord*, float, Layout*, Item*[5]);
    virtual ~Level();

    Coord *spawnCoord;
    Coord *exitCoord;
    float spawnA;
    Layout *levelMap;
    Item *items[5];

    virtual void update(Game*);

  protected:

  private:
};

class Level1 : public Level {
    public:
        Level1();
        Level1(Coord*, Coord*, float, Layout*, Item*[5]);
        virtual ~Level1();

        void update(Game*);

    protected:

    private:
};

class Level2 : public Level {
    public:
        Level2();
        Level2(Coord*, Coord*, float, Layout*, Item*[5]);
        virtual ~Level2();

        void update(Game*);

    protected:

    private:
};

class Level3 : public Level {
    public:
        Level3();
        Level3(Coord*, Coord*, float, Layout*, Item*[5]);
        virtual ~Level3();

        void update(Game*);

    protected:

    private:
};

class Level4 : public Level {
    public:
        Level4();
        Level4(Coord*, Coord*, float, Layout*, Item*[5]);
        virtual ~Level4();

        void update(Game*);

    protected:

    private:
};


#endif // LEVEL_H
