#ifndef GAME_H
#define GAME_H

// I include all of these things because I like bells and whistles
// I also like stealing other people's work
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <conio.h>
#include <math.h>

#include <chrono>
#include <thread>
#include <future>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <mmsystem.h>

#include "Level.h"
#include "Person.h"
#include "Monster.h"
#include "Item.h"
#include "Potion.h"
#include "Stack.h"

class Game
{
    public:
        Game();
        Game(Person*);
        virtual ~Game();

        HANDLE hConsole;
        DWORD dwBytesWritten;

        template <class Rep, class Period>
        void sleep_for(const std::chrono::duration<Rep,Period>& rel_time);

        // Function definitions
        void display();

        void colors(int);
        bool approxEqual(float, float, float);
        void revealMap();
        void solve();
        void rayCast();
        bool castOneRay(float, float, char, bool);

        Person *player;

        Layout *currentCaption;
        Layout *fogOfWar;

        Layout *lookingAt;

        int nScreenWidth;			// Console Screen Size X (columns)
        int nScreenHeight;			// Console Screen Size Y (rows)

        float fDepth;			// Maximum rendering distance
        int FOVmultiplier;

        float fElapsedTime;

        bool debug;


        bool autoSolve;
        int lastPop;
        Node *n;
        Node *n2;
        Stack *autoMove;
        Stack *backTrackStack;

        Level *lvl1;
        Level *lvl2;
        Level *lvl3;
        Level *lvl4;
        Level *currentLevel;

        char *screen;

        ofstream config;

        Monster1 *mnstr1;
        Monster2 *mnstr2;
        Monster2 *mnstr3;
        Monster2 *mnstr4;

        Layout *swordLayout;
        Layout *swordStrikeLayout;
        bool strike;

        Item *note1;
        Item *note2;
        Item *key;
        Item *sword;
        Item *drunkPotion;
        Item *healthPotion;

        Item *lvl1Gate;
        Item *lvl2Gate;

        bool quit;

    protected:

    private:

};

#endif // GAME_H
