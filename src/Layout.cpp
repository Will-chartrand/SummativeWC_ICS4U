#include "Layout.h"

Layout::Layout()
{
    mapString = L"";
}

Layout::Layout(int _width, int _height, int _xpos, int _ypos)
{
    width = _width;
    height = _height;
    xpos = _xpos;
    ypos = _ypos;
    mapString = L"";
}

Layout::~Layout()
{
    //dtor
}

int Layout::getWidth(){
    return width;
}

int Layout::getHeight(){
    return height;
}

void Layout::displayLayout(char screen[], int &nScreenWidth, int &nScreenHeight){
    for (int nx = 0; nx < width; nx++){
        for (int ny = 0; ny < height; ny++) {
                screen[(ny+ypos)*nScreenWidth + nx + xpos] = mapString[ny * width + nx];
                //screen[(ny+1)*nScreenWidth + nx] = map[nMap][ny * nMapWidth + nx];
            }
    }
    return;
}
