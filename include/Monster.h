#ifndef MONSTER_H
#define MONSTER_H

#include "Being.h"

class Game;

class Monster : public Being {
  public:
    Monster();
    Monster(float, Coord*);
    virtual ~Monster();

    virtual void moveMonster(Game*);

  protected:

  private:
};

class Monster1 : public Monster {
public:
    Monster1();
    Monster1(float, Coord*);
    virtual ~Monster1();

    void moveMonster(Game*);
};

class Monster2 : public Monster {
public:
    Monster2();
    Monster2(float, Coord*);
    virtual ~Monster2();

    void moveMonster(Game*);
};
#endif // MONSTER_H
