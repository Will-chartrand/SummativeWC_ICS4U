#ifndef BEING_H
#define BEING_H

#include <windows.h>
#include <math.h>

#include "Coord.h"

class Being{
    public:
        Being();
        Being(float, float, float, Coord*);
        virtual ~Being();

        Coord *beingCoord;
        Coord *prevBeingCoord;
        float fPlayerA;			// Player Start Rotation
        float fFOV;
        float fSpeed;			// Walking Speed

        bool dead;

    protected:

    private:
};

#endif // BEING_H
