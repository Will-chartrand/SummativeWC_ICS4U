#ifndef POTION_H
#define POTION_H

#include "Item.h"

class Game;
class Potion : public Item{
  public:
    Potion();
    Potion(char, Coord*);
    virtual ~Potion();

    virtual void effect(Game*);

  protected:

  private:
};

class Potion1 : public Potion{
  public:
    Potion1(char, Coord*);
    virtual ~Potion1();

    void effect(Game*);

  protected:

  private:
};

class Potion2 : public Potion{
  public:
    Potion2(char, Coord*);
    virtual ~Potion2();

    void effect(Game*);

  protected:

  private:
};


#endif // POTION_H
