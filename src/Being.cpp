#include "Being.h"

Being::Being(){

}

Being::Being(float angle, float FOV, float speed, Coord *startCoord){
    fPlayerA = angle;			// Player Start Rotation
    fFOV = FOV;	// Field of View
    fSpeed = speed;			// Walking Speed
    beingCoord = startCoord;
    dead = false;
}


Being::~Being(){
    //dtor
}
