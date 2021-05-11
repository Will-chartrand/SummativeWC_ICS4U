#ifndef ITEM_H
#define ITEM_H
#include "Coord.h"

class Game;
class Item
{
    public:
        Item();
        Item(char, Coord*);
        virtual ~Item();

        virtual void effect(Game*);

        char character;
        Coord *location;
        bool has;

    protected:

    private:
};

#endif // ITEM_H
