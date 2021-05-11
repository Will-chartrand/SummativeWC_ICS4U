#ifndef PERSON_H
#define PERSON_H

#include "Monster.h"
#include "Layout.h"

class Game;

class Person : public Being {
  public:
    Person();
    Person(float, float, float, Layout*, Coord*);
    virtual ~Person();

    void update(Game*);

    void SWING_DA_SWORD(Game*);

    Layout *inv;
    int health;
    Layout *healthLayout;

    char onCurrentTile;

  protected:

  private:
};

#endif // PERSON_H
