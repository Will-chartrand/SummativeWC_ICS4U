/**

     Made by Will Chartrand
     Due date: 08/04/21 (DD/NM/YY)
     Hand-in date: Probably after 08/04/21

     This is the final project for the ICS4U course

     Assignment:
     ==========================================================================
     Your task is to create a game that requires the user to navigate a maze
     and interact with objects and other beings inside the maze.
     The game should end when either the user’s character dies or the character
     reaches the exit of the maze.
     ==========================================================================

     Credit for the bit of code is at the bottom of this file

**/

#include "Game.h"

using namespace std;

int main() {
    Person *player = new Person(0.0f, 3.14159f/3.0f, 3.0f, new Layout(10, 5, 0, 20), nullptr);

    Game game(player);
    Game *ptr_game = &game;

    const int nMapWidth = 15;
    const int nMapHeight = 15;

    // Set window size
    system("mode con: cols=120 lines=40");

    // Set window position
    HWND consoleWindow = GetConsoleWindow();
    SetWindowPos( consoleWindow, 0, 100, 40, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

    // Get this fancy handle for the console so that displaying the screen is possible
    game.hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(game.hConsole);
    game.dwBytesWritten = 0;

    // We'll need time differential per frame to calculate modification
    // to movement speeds, to ensure consistant movement, as ray-tracing
    // is non-deterministic
    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    std::ifstream openConfig("config.txt");
    std::string configOutput;


    // Initializing autosolve stacks
    game.n = new Node(0, nullptr);
    game.n2 = new Node(0, nullptr);
    game.autoMove = new Stack(game.n, game.n);
    game.backTrackStack = new Stack(game.n2, game.n2);
    game.autoMove->push(0);
    game.backTrackStack->push(game.autoMove->pop());      // push the backtracking move
    game.autoMove->push(4);
    game.autoMove->push(3);
    game.autoMove->push(2);
    game.autoMove->push(1);
    game.autoMove->push(0);

    char onCurrentTile = ' ';

    Layout startMenu(10, 5, 20, 10);
    startMenu.mapString += L"New Game  ";
    startMenu.mapString += L"Load Game ";
    startMenu.mapString += L"ReadMe    ";
    startMenu.mapString += L"Exit      ";
    startMenu.mapString += L"Solve     ";


    string startMenuArrow = ">   "; //each position will be diplayed vertically, next to each option
    int arrowPos = 0;

    // Create Map of world space
    //  # = wall
    //  I = gate
    //  @ = portal to next level
    //  k = key
    //  s = sword
    //  8 = potion
    //  n = note

    Layout *map1Layout = new Layout(nMapWidth, nMapHeight, 0, 1);

    map1Layout->mapString += L"###############";
    map1Layout->mapString += L"#n  #*  #     #";
    map1Layout->mapString += L"### ###   ### #";
    map1Layout->mapString += L"# #   ### #   #";
    map1Layout->mapString += L"# # ### # # #8#";
    map1Layout->mapString += L"#   # # # # ###";
    map1Layout->mapString += L"# #       #   #";
    map1Layout->mapString += L"# # #####    ##";
    map1Layout->mapString += L"# # #   #######";
    map1Layout->mapString += L"# ### # # #   #";
    map1Layout->mapString += L"# #   # # # # #";
    map1Layout->mapString += L"# # ###       #";
    map1Layout->mapString += L"# # # ##### ###";
    map1Layout->mapString += L"#     k#@  I# #";
    map1Layout->mapString += L"###############";

    Layout *map2Layout = new Layout(nMapWidth, nMapHeight, 0, 1);

    map2Layout->mapString += L"###############";
    map2Layout->mapString += L"#   #*        #";
    map2Layout->mapString += L"############# #";
    map2Layout->mapString += L"#             #";
    map2Layout->mapString += L"#### ####  # ##";
    map2Layout->mapString += L"#  # # ###   ##";
    map2Layout->mapString += L"#    #  #######";
    map2Layout->mapString += L"#  ###  n     #";
    map2Layout->mapString += L"#             #";
    map2Layout->mapString += L"#   ######  ###";
    map2Layout->mapString += L"#     #  ##   #";
    map2Layout->mapString += L"####  #       #";
    map2Layout->mapString += L"#    ##  ######";
    map2Layout->mapString += L"#     #s     @#";
    map2Layout->mapString += L"###############";

    Layout *map3Layout = new Layout(nMapWidth, nMapHeight, 0, 1);

    map3Layout->mapString += L"###############";
    map3Layout->mapString += L"#   *         #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#             #";
    map3Layout->mapString += L"#           #I#";
    map3Layout->mapString += L"#           #@#";
    map3Layout->mapString += L"###############";

    Layout *map4Layout = new Layout(nMapWidth, nMapHeight, 0, 1);
    map4Layout->mapString += L"###############";
    map4Layout->mapString += L"#* #          #";
    map4Layout->mapString += L"#  #   # #### #";
    map4Layout->mapString += L"#  #  ##    # #";
    map4Layout->mapString += L"#  #   #### ###";
    map4Layout->mapString += L"#  ##  #      #";
    map4Layout->mapString += L"#      # # ####";
    map4Layout->mapString += L"# ###### #    #";
    map4Layout->mapString += L"# #8  ######  #";
    map4Layout->mapString += L"# ### #       #";
    map4Layout->mapString += L"# ##  ####### #";
    map4Layout->mapString += L"# #         # #";
    map4Layout->mapString += L"# ########  # #";
    map4Layout->mapString += L"#          # @#";
    map4Layout->mapString += L"###############";

    game.fogOfWar = new Layout(nMapWidth, nMapHeight, 0, 1);
    game.fogOfWar->mapString =  L"###############";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"#XXXXXXXXXXXXX#";
    game.fogOfWar->mapString += L"###############";


    Layout *computerLayout = new Layout(56, 18, 9, 5);

    computerLayout->mapString += L"             ,----------------,              ,---------,";
    computerLayout->mapString += L"        ,-----------------------,          ,\"        ,\"|";
    computerLayout->mapString += L"      ,\"                      ,\"|        ,\"        ,\"  |";
    computerLayout->mapString += L"     +-----------------------+  |      ,\"        ,\"    |";
    computerLayout->mapString += L"     |  .-----------------.  |  |     +---------+      |";
    computerLayout->mapString += L"     |  |                 |  |  |     | -==----'|      |";
    computerLayout->mapString += L"     |  |                 |  |  |     |         |      |";
    computerLayout->mapString += L"     |  |                 |  |  |/----|`---=    |      |";
    computerLayout->mapString += L"     |  |                 |  |  |   ,/|==== ooo |      ;";
    computerLayout->mapString += L"     |  |                 |  |  |  // |(((( [33]|    ,\" ";
    computerLayout->mapString += L"     |  `-----------------'  |,\" .;'| |((((     |  ,\"   ";
    computerLayout->mapString += L"     +-----------------------+  ;;  | |         |,\"     ";
    computerLayout->mapString += L"        /_)______________(_/  //'   | +---------+       ";
    computerLayout->mapString += L"   ___________________________/___  `,                  ";
    computerLayout->mapString += L"  /  oooooooooooooooo  .o.  oooo /,   \\,\"-----------    ";
    computerLayout->mapString += L" / ==ooooooooooooooo==.o.  ooo= //   ,`\\--{)B     ,\"    ";
    computerLayout->mapString += L"/_==__==========__==_ooo__ooo=_/'   /___________,\"      ";
    computerLayout->mapString += L"`-----------------------------' Artwork - kevin lam     ";


    game.swordLayout = new Layout(11, 8, game.nScreenWidth - 11, game.nScreenHeight - 8);

    game.swordLayout->mapString += L"      ^    ";
    game.swordLayout->mapString += L"     | |   ";
    game.swordLayout->mapString += L"     | |   ";
    game.swordLayout->mapString += L"     | |   ";
    game.swordLayout->mapString += L"     | |   ";
    game.swordLayout->mapString += L"     | |   ";
    game.swordLayout->mapString += L"    _____  ";
    game.swordLayout->mapString += L"     | |   ";

    game.swordStrikeLayout = new Layout(11, 4, game.nScreenWidth - 11, game.nScreenHeight - 4);

    game.swordStrikeLayout->mapString += L" < \\       ";
    game.swordStrikeLayout->mapString += L"  \\ \\      ";
    game.swordStrikeLayout->mapString += L"    ____   ";
    game.swordStrikeLayout->mapString += L"     \\ \\   ";

    game.currentCaption = new Layout(60, 2, 30, 5);

    game.lookingAt = new Layout(8, 1, 53, game.nScreenHeight - 2);
    game.lookingAt->mapString += L"        ";

    game.player->healthLayout = new Layout(11, 1, 0, 27);
    game.player->healthLayout->mapString = L"Health:   ";

    bool startGame = false;
    game.quit = false;

    bool playFootSteps = false;

    game.note1 = new Item('n', new Coord(1.5f, 1.5f));
    game.note2 = new Item('n', new Coord(8.5f, 7.5f));
    game.key = new Item('k', new Coord(6.5f, 13.5f));
    game.sword = new Item('s', new Coord(3.5f, 10.5f));
    game.drunkPotion = new Potion1('8', new Coord(13.5f, 4.5f));
    game.healthPotion = new Potion2('8', new Coord(3.5f, 8.5f));

    game.lvl1Gate = new Item('I', new Coord(11.5f, 13.5f));
    game.lvl2Gate = new Item('I', new Coord(12.5f, 13.5f));

    Item *lvl1Items[5] = {game.key, game.lvl1Gate, game.note1, game.drunkPotion, nullptr};
    Item *lvl2Items[5] = {game.note2, game.sword, nullptr, nullptr, nullptr};
    Item *lvl3Items[5] = {game.key, game.lvl2Gate, nullptr, nullptr, nullptr};
    Item *lvl4Items[5] = {game.healthPotion, nullptr, nullptr, nullptr, nullptr};

    game.lvl1 = new Level1(new Coord(1.5f, 5.5f), new Coord(13.5f, 8.5f), 0.0f, map1Layout, lvl1Items);
    game.lvl2 = new Level2(new Coord(1.5f, 5.5f), new Coord(13.5f, 13.5f), 0.0f, map2Layout, lvl2Items);
    game.lvl3 = new Level3(new Coord(1.5f, 5.5f), new Coord(13.5f, 13.5f), 1.5f, map3Layout, lvl3Items);
    game.lvl4 = new Level4(new Coord(1.5f, 1.5f), new Coord(13.5f, 13.5f), 1.5f, map4Layout, lvl4Items);

    game.currentLevel = game.lvl1;

    game.mnstr1 = new Monster1(0.5f, new Coord(10,10));
    game.mnstr2 = new Monster2(1.0f, new Coord(3,3));
    game.mnstr3 = new Monster2(1.0f, new Coord(6,3));
    game.mnstr4 = new Monster2(1.0f, new Coord(1,5));

    game.mnstr2->fSpeed = 1.0f;


    for(int y = 0; y < game.nScreenHeight; y++)
        for(int x = 0; x < game.nScreenWidth; x++)
            game.screen[x*y] = ' ';


    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    std::cout << "Would you like some moody music to play this game?(y/n)\n";
    if(getch() == 'y') {
        system("start https://www.youtube.com/watch?v=Lf099gm1Mz8");
    }
    system("CLS");
    SetConsoleActiveScreenBuffer(game.hConsole);

    //display computer
    computerLayout->displayLayout(game.screen, game.nScreenWidth, game.nScreenHeight);

    // Menu
    while (!startGame) {
        //display start menu
        startMenu.displayLayout(game.screen, game.nScreenWidth, game.nScreenHeight);

        for(int i = 0; i < 5; i++)
            game.screen[(i + 10)*game.nScreenWidth + 18] = startMenuArrow[i];

        // Display Frame
        game.screen[game.nScreenWidth * game.nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(game.hConsole, game.screen, game.nScreenWidth * game.nScreenHeight, { 0,0 }, &(game.dwBytesWritten));

        switch(getch()) {
        case 'x':
            game.debug = true;
            break;
        case 'w':
            startMenuArrow[arrowPos] = ' ';
            arrowPos--;
            if(arrowPos < 0)
                arrowPos = 4;
            break;

        case 's':
            startMenuArrow[arrowPos] = ' ';
            arrowPos++;
            break;

        case 'd':
            this_thread::sleep_for(chrono::milliseconds(150));
            switch(arrowPos) {
            case 0:
                startGame = true;
                break;
            case 1:
                if(openConfig.is_open()) {
                    getline(openConfig, configOutput);
                    openConfig.close();

                    if(configOutput == "1")
                        game.currentLevel = game.lvl1;
                    if(configOutput == "2")
                        game.currentLevel = game.lvl2;
                    if(configOutput == "3")
                        game.currentLevel = game.lvl3;
                    if(configOutput == "4")
                        game.currentLevel = game.lvl4;
                }

                startGame = true;
                break;

            case 2:
                SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
                system("start notes/welcome.txt");
                SetConsoleActiveScreenBuffer(game.hConsole);
                break;
            case 3:
                startGame = true;
                game.quit = true;
                break;
            case 4:
                game.autoSolve = true;
                //game.autoMove->push(0);

                startGame = true;
                break;
            default:
                break;
            }
        }

        if(!game.debug) {
            PlaySound(TEXT("sounds/click.wav"), NULL, SND_FILENAME);
        }

        arrowPos %= 5;
        startMenuArrow[arrowPos] = '>';
    }

    thread playStartUp([&]() {
        if(!game.debug) {
            PlaySound(TEXT("sounds/startup.wav"), NULL, SND_FILENAME);
        }

    });

    playStartUp.detach();
    FlushConsoleInputBuffer(game.hConsole);


//==================================================================================================================

    thread playStepsThread([&] {
        while(!game.quit) {
            if(playFootSteps) {
                if(!game.debug)
                    PlaySound(TEXT("sounds/playerSteps.wav"), NULL, SND_LOOP);
            }
        }

    });


    //======================================
    game.player->beingCoord = new Coord(game.currentLevel->spawnCoord->x, game.currentLevel->spawnCoord->y);
    game.player->prevBeingCoord = new Coord(game.player->beingCoord->x, game.player->beingCoord->y);

    game.currentCaption->mapString  = L"            You find yourself in a maze                     ";
    game.currentCaption->mapString += L"               How did you get here?                        ";


    while(!game.quit) {
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        game.fElapsedTime = elapsedTime.count();


        if(game.autoSolve) {
            game.currentCaption->mapString  = L"                Hold 'W' to autosolve                       ";
            game.currentCaption->mapString += L"                                                            ";

            if((GetAsyncKeyState((unsigned short)'W') & 0x8000)) {
                game.solve();
            }

        } else {
            game.player->update(ptr_game);
            game.currentLevel->update(ptr_game);

        }

        // Lift the fog of war
        game.revealMap();

        if (GetAsyncKeyState((unsigned short)VK_ESCAPE) & 0x8000 || game.player->dead) {
            game.quit = true;
        }


        if(!(GetAsyncKeyState((unsigned short)'A') & 0x8000 || GetAsyncKeyState((unsigned short)'W') & 0x8000 || GetAsyncKeyState((unsigned short)'S') & 0x8000 || GetAsyncKeyState((unsigned short)'D') & 0x8000)) {
            playFootSteps = false;
        } else {
            playFootSteps = true;
        }


        game.rayCast();
        game.display();

        // Display Stats
        //swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", game.player->beingCoord->x, game.player->beingCoord->y, fPlayerA, 1.0f/game.fElapsedTime);
//        SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
//        cout << "\r" << sinf(player->fPlayerA) * player->fSpeed * game.fElapsedTime << "  " << cosf(player->fPlayerA) * player->fSpeed * game.fElapsedTime;
//        SetConsoleActiveScreenBuffer(game.hConsole);
    }

    game.player->dead = true;

    // Store last level in a file
    game.config.open("config.txt");
    if(game.currentLevel == game.lvl1) {
        game.config << "1";
    } else if(game.currentLevel == game.lvl2) {
        game.config << "2";
    } else if(game.currentLevel == game.lvl3) {
        game.config << "3";
    } else if(game.currentLevel == game.lvl4) {
        game.config << "4";
    }
    game.config.close();

    return 0;
}


/*
For the code that I stole:

	OneLoneCoder.com - Command Line First Person Shooter (FPS) Engine
	"Why were games not done like this is 1990?" - @Javidx9
	License
	~~~~~~~
	Copyright (C) 2018  Javidx9
	This program comes with ABSOLUTELY NO WARRANTY.
	This is free software, and you are welcome to redistribute it
	under certain conditions; See license for details.
	Original works located at:
	https://www.github.com/onelonecoder
	https://www.onelonecoder.com
	https://www.youtube.com/javidx9
	GNU GPLv3
	https://github.com/OneLoneCoder/videos/blob/master/LICENSE
	From Javidx9 :)
	~~~~~~~~~~~~~~~
	Hello! Ultimately I don't care what you use this for. It's intended to be
	educational, and perhaps to the oddly minded - a little bit of fun.
	Please hack this, change it and use it in any way you see fit. You acknowledge
	that I am not responsible for anything bad that happens as a result of
	your actions. However this code is protected by GNU GPLv3, see the license in the
	github repo. This means you must attribute me if you use it. You can view this
	license here: https://github.com/OneLoneCoder/videos/blob/master/LICENSE
	Cheers!
	Background
	~~~~~~~~~~
	Whilst waiting for TheMexicanRunner to start the finale of his NesMania project,
	his Twitch stream had a counter counting down for a couple of hours until it started.
	With some time on my hands, I thought it might be fun to see what the graphical
	capabilities of the console are. Turns out, not very much, but hey, it's nice to think
	Wolfenstein could have existed a few years earlier, and in just ~200 lines of code.
	IMPORTANT!!!!
	~~~~~~~~~~~~~
	READ ME BEFORE RUNNING!!! This program expects the console dimensions to be set to
	120 Columns by 40 Rows. I recommend a small font "Consolas" at size 16. You can do this
	by running the program, and right clicking on the console title bar, and specifying
	the properties. You can also choose to default to them in the future.

	Controls: A = Turn Left, D = Turn Right, W = Walk Forwards, S = Walk Backwards
	Future Modifications
	~~~~~~~~~~~~~~~~~~~~
	1) Shade block segments based on angle from player, i.e. less light reflected off
	#s at side of game.player-> #s straight on are brightest.
	2) Find an interesting and optimised ray-tracing method. I'm sure one must exist
	to more optimally search the map space
	3) Add bullets!
	4) Add bad guys!
	Author
	~~~~~~
	Twitter: @javidx9
	Blog: www.onelonecoder.com
	Video:
	~~~~~~
	https://youtu.be/xW8skO7MFYw
	Last Updated: 27/02/2017
*/

/*
    I also decided to throw in Desktop Goose in here because
    I thought it'd be funny, and it definitely is
*/
