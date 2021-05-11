#include "Game.h"

Game::Game() {
    nScreenWidth = 120;			// Console Screen Size X (columns)
    nScreenHeight = 40;			// Console Screen Size Y (rows)

    fDepth = 16.0f;			// Maximum rendering distance
    FOVmultiplier = 0;

    autoSolve = false;

    dwBytesWritten = 0;

    screen = new char[nScreenWidth*nScreenHeight];

}

Game::Game(Person *_player) {
    nScreenWidth = 120;			// Console Screen Size X (columns)
    nScreenHeight = 40;			// Console Screen Size Y (rows)

    fDepth = 16.0f;			// Maximum rendering distance
    FOVmultiplier = 0;

    autoSolve = false;

    dwBytesWritten = 0;
    player = _player;

    debug = false;

    strike = false;

    screen = new char[nScreenWidth*nScreenHeight];
}

Game::~Game() {
    //dtor
}

void Game::display() {

    bool displayView = true;
    std::wstring addHealth= L"";

    // Display Map
    if(debug || autoSolve) {
        currentLevel->levelMap->displayLayout(screen, nScreenWidth, nScreenHeight);
    } else {
        fogOfWar->displayLayout(screen, nScreenWidth, nScreenHeight);
    }



    // Bread Crumbs
    //currentLevel->levelMap->mapString[((int)player->beingCoord->x) * currentLevel->levelMap->getWidth() + (int)player->beingCoord->y] = '*';

    // Write person to Screen
    screen[((int)player->beingCoord->x+1) * nScreenWidth + (int)player->beingCoord->y] = 'P';

    // Display Inventory
    player->inv->displayLayout(screen, nScreenWidth, nScreenHeight);

    // Display Caption
    currentCaption->displayLayout(screen, nScreenWidth, nScreenHeight);

    // Display Sword if in inventory
    if(sword->has) {
        if(strike)
            swordStrikeLayout->displayLayout(screen, nScreenWidth, nScreenHeight);
        else
            swordLayout->displayLayout(screen, nScreenWidth, nScreenHeight);
    }

    // Display name of what's directly infront of you
    if(castOneRay(2.0f, player->fPlayerA, 's', false)) {
        lookingAt->mapString = L" Sword  ";
    } else if(castOneRay(2.0f, player->fPlayerA, '8', false)) {
        lookingAt->mapString = L" Potion ";
    } else if(castOneRay(2.0f, player->fPlayerA, 'n', false)) {
        lookingAt->mapString = L"  Note  ";
    } else if(castOneRay(2.0f, player->fPlayerA, 'k', false)) {
        lookingAt->mapString = L"  Key   ";
    } else if(castOneRay(2.0f, player->fPlayerA, 'I', false)) {
        lookingAt->mapString = L"  Door  ";
    } else if(castOneRay(2.0f, player->fPlayerA, '@', false)) {
        lookingAt->mapString = L"  Gate  ";
    } else if(castOneRay(2.0f, player->fPlayerA, '#', false)) {
        lookingAt->mapString = L"  Wall  ";
    } else if(castOneRay(1.0f, player->fPlayerA, 'e', false)) {
            lookingAt->mapString = L" Strike ";
    } else if(castOneRay(2.0f, player->fPlayerA, 'e', false)) {
        lookingAt->mapString = L" Enemy  ";
    } else {
        displayView = false;
    }

    if(displayView)
        lookingAt->displayLayout(screen, nScreenWidth, nScreenHeight);

    // Display Health
    player->healthLayout->mapString = L"Health: ";
    for(int i = 0; i < 3; i++) {
        if(player->health - i > 0) {
            addHealth += 207;
        } else {
            addHealth += L" ";
        }
    }
    player->healthLayout->mapString += addHealth;
    player->healthLayout->displayLayout(screen, nScreenWidth, nScreenHeight);

    // Display Frame
    screen[nScreenWidth * nScreenHeight - 1] = '\0';
    WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);

    return;
}
void Game::colors(int k) {
    // Never actally got around to figuring colours out. Would have been cool :(
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}

bool Game::approxEqual(float a, float b, float difference) {
    if(abs(a - b) < difference) {
        return true;
    } else {
        return false;
    }
}

void Game::revealMap() {
    // Everything to the North, East, South, West and on where the player is standing gets revealed
    fogOfWar->mapString[((int)player->beingCoord->x) * fogOfWar->getWidth() + (int)player->beingCoord->y] =
        currentLevel->levelMap->mapString[((int)player->beingCoord->x) * fogOfWar->getWidth() + (int)player->beingCoord->y];

    fogOfWar->mapString[((int)player->beingCoord->x) * fogOfWar->getWidth() + (int)player->beingCoord->y - 1] =
        currentLevel->levelMap->mapString[((int)player->beingCoord->x) * fogOfWar->getWidth() + (int)player->beingCoord->y - 1];

    fogOfWar->mapString[((int)player->beingCoord->x) * fogOfWar->getWidth() + (int)player->beingCoord->y + 1] =
        currentLevel->levelMap->mapString[((int)player->beingCoord->x) * fogOfWar->getWidth() + (int)player->beingCoord->y + 1];

    fogOfWar->mapString[((int)player->beingCoord->x - 1) * fogOfWar->getWidth() + (int)player->beingCoord->y] =
        currentLevel->levelMap->mapString[((int)player->beingCoord->x - 1) * fogOfWar->getWidth() + (int)player->beingCoord->y];

    fogOfWar->mapString[((int)player->beingCoord->x + 1) * fogOfWar->getWidth() + (int)player->beingCoord->y] =
        currentLevel->levelMap->mapString[((int)player->beingCoord->x + 1) * fogOfWar->getWidth() + (int)player->beingCoord->y];
    return;
}


void Game::rayCast() {
    char nShade = ' ';
    bool hittingObj = false;

    for (int x = 0; x < nScreenWidth; x++) {
        // For each column, calculate the projected ray angle into world space
        float fRayAngle = (player->fPlayerA - player->fFOV/2.0f) + ((float)x / (float)nScreenWidth) * player->fFOV;

        // Find distance to #
        float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase
        float fDistanceToWall = 0.0f; //                                      resolution

        bool bHitWall = false;		// Set when ray hits # block
        bool bBoundary = false;		// Set when ray hits boundary between two # blocks

        float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
        float fEyeY = cosf(fRayAngle);

        // Incrementally cast ray from player, along ray angle, testing for
        // intersection with a block
        while (!bHitWall && fDistanceToWall < fDepth) {
            fDistanceToWall += fStepSize;
            int nTestX = (int)(player->beingCoord->x + fEyeX * fDistanceToWall);
            int nTestY = (int)(player->beingCoord->y + fEyeY * fDistanceToWall);

            // Test if ray is out of bounds
            if (nTestX < 0 || nTestX >= currentLevel->levelMap->getWidth() || nTestY < 0 || nTestY >= currentLevel->levelMap->getHeight()) {
                bHitWall = true;			// Just set distance to maximum depth
                fDistanceToWall = fDepth;
            }

            else {
                switch(currentLevel->levelMap->mapString.c_str()[nTestX * currentLevel->levelMap->getWidth() + nTestY]) {
                case '#':
                case 'I':
                case '@':
                case 'n':
                case 'k':
                case 's':
                case '8':
                case 'e': {
                    // Ray has hit #
                    bHitWall = true;

                    // To highlight tile boundaries, cast a ray from each corner
                    // of the tile, to the player-> The more coincident this ray
                    // is to the rendering ray, the closer we are to a tile
                    // boundary, which we'll shade to add detail to the #s
                    vector<pair<float, float>> p;

                    // Test each corner of hit tile, storing the distance from
                    // the player, and the calculated dot product of the two rays
                    for (int tx = 0; tx < 2; tx++)
                        for (int ty = 0; ty < 2; ty++) {
                            // Angle of corner to eye
                            float vy = (float)nTestY + ty - player->beingCoord->y;
                            float vx = (float)nTestX + tx - player->beingCoord->x;
                            float d = sqrt(vx*vx + vy*vy);
                            float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
                            p.push_back(make_pair(d, dot));
                        }

                    // Sort Pairs from closest to farthest
                    sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {
                        return left.first < right.first;
                    });

                    // First two/three are closest (we will never see all four)
                    float fBound = 0.01;
                    if (acos(p.at(0).second) < fBound)
                        bBoundary = true;
                    if (acos(p.at(1).second) < fBound)
                        bBoundary = true;
                    if (acos(p.at(2).second) < fBound)
                        bBoundary = true;
                    break;
                }

                default:
                    break;
                }

                switch(currentLevel->levelMap->mapString.c_str()[nTestX * currentLevel->levelMap->getWidth() + nTestY]) {
                case '@':
                    nShade = '@';
                    hittingObj = true;
                    break;

                case 'I':
                    nShade = 'I';
                    hittingObj = true;
                    break;

                case 'k':
                    nShade = 'k';
                    hittingObj = true;
                    break;

                case 's':
                    nShade = 's';
                    hittingObj = true;
                    break;

                case '8':
                    nShade = '8';
                    hittingObj = true;
                    break;

                case 'e':
                    nShade = 1;
                    hittingObj = true;
                    break;
                case 'n':
                    nShade = 'N';
                    hittingObj = true;
                    break;

                default:
                    hittingObj = false;
                    break;
                }

            }
        }

        // Calculate distance to ceiling and floor
        int nCeiling = (float)(nScreenHeight/2.0) - nScreenHeight / ((float)fDistanceToWall);
        int nFloor = nScreenHeight - nCeiling;

        // Shader #s based on distance
        if(!hittingObj) {
            if (fDistanceToWall <= fDepth / 10.0f) {
                nShade = 219;	// Very close
            } else if (fDistanceToWall < fDepth / 8.0f) {
                nShade = 178;
            } else if (fDistanceToWall < fDepth / 6.0f) {
                nShade = 177;
            } else if (fDistanceToWall < fDepth / 2.5f) {
                nShade = 176;
            } else if (fDistanceToWall < fDepth) {
                nShade = '.';
            } else {
                nShade = ' ';		// Too far away
            }
        }

        if (bBoundary)
            nShade = ' '; // Black it out

        for (int y = 0; y < nScreenHeight; y++) {
            // Each Row
            if(y <= nCeiling)
                screen[y*nScreenWidth + x] = ' ';
            else if(y > nCeiling && y <= nFloor)
                screen[y*nScreenWidth + x] = nShade;
            else { // Floor
                // Shade floor based on distance
                float b = 1.0f - (((float)y - nScreenHeight/2.0f) / ((float)nScreenHeight / 2.0f));
                if (b < 0.25)
                    nShade = '#';
                else if (b < 0.5)
                    nShade = 'x';
                else if (b < 0.75)
                    nShade = '.';
                else if (b < 0.9)
                    nShade = '-';
                else
                    nShade = ' ';
                screen[y*nScreenWidth + x] = nShade;
            }
        }
    }
    return;
}

bool Game::castOneRay(float maxDistance, float fAngle, char target, bool kill) {

    float fRayAngle = (fAngle/* - player->fFOV/2.0f*/);

    // Find distance to #
    float fStepSize = 0.05f;		  // Increment size for ray casting, decrease to increase
    float fDistanceToWall = 0.0f; //                                      resolution

    bool bHitWall = false;		// Set when ray hits # block

    float fEyeX;
    float fEyeY;
    int nTestX;
    int nTestY;


    // Incrementally cast ray from player, along ray angle, testing for
    // intersection with a block
    while (!bHitWall && fDistanceToWall < maxDistance) {
        fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
        fEyeY = cosf(fRayAngle);
        fDistanceToWall += fStepSize;
        nTestX = (int)(player->beingCoord->x + fEyeX * fDistanceToWall);
        nTestY = (int)(player->beingCoord->y + fEyeY * fDistanceToWall);

        // Test if ray is out of bounds
        if (nTestX < 0 || nTestX >= currentLevel->levelMap->getWidth() || nTestY < 0 || nTestY >= currentLevel->levelMap->getHeight()) {
            bHitWall = true;			// Just set distance to maximum depth
            fDistanceToWall = fDepth;
        } else if(currentLevel->levelMap->mapString.c_str()[nTestX * currentLevel->levelMap->getWidth() + nTestY] == target) {
            {
                if(kill && fDistanceToWall < 1.0f) {
                    if(nTestX == (int)mnstr1->beingCoord->x && nTestY == (int)mnstr1->beingCoord->y) {
                        mnstr1->dead = true;
                        currentLevel->levelMap->mapString[mnstr1->beingCoord->x * currentLevel->levelMap->getWidth() + mnstr1->beingCoord->y] = ' ';
                    }
                    if(nTestX == (int)mnstr2->beingCoord->x && nTestY == (int)mnstr2->beingCoord->y) {
                        mnstr2->dead = true;
                        currentLevel->levelMap->mapString[mnstr2->beingCoord->x * currentLevel->levelMap->getWidth() + mnstr2->beingCoord->y] = ' ';
                    }
                    if(nTestX == (int)mnstr3->beingCoord->x && nTestY == (int)mnstr3->beingCoord->y) {
                        mnstr3->dead = true;
                        currentLevel->levelMap->mapString[mnstr3->beingCoord->x * currentLevel->levelMap->getWidth() + mnstr3->beingCoord->y] = ' ';
                    }
                    if(nTestX == (int)mnstr4->beingCoord->x && nTestY == (int)mnstr4->beingCoord->y) {
                        mnstr4->dead = true;
                        currentLevel->levelMap->mapString[mnstr4->beingCoord->x * currentLevel->levelMap->getWidth() + mnstr4->beingCoord->y] = ' ';
                    }
                }
                // Ray has hit #
                return true;
            }
        }
    }
    return false;
}

void Game::solve() {

    // Sorry this is such a c h u n k y autosolver
    //==========
    // Possible stack pops
    // 0 = East
    // 1 = South
    // 2 = West
    // 3 = North
    // 4 = Back Tracking
    // -2 = popping from empty stack
    // -1 = peeking from empty stack
    //==========


    if(autoMove->peek() == 4) {
        autoMove->pop();
        autoMove->push((backTrackStack->pop() + 2) % 4); // The + 2 % 4 gets the opposite direction
    }
    float targetAngle = autoMove->peek() * 3.14159f/2.0f; //measuring degrees
    bool pushMoves = false;

//    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
//    cout << "\r" << targetAngle;
//    SetConsoleActiveScreenBuffer(hConsole);

    if(GetAsyncKeyState((unsigned short)'B') & 0x8000) {
        SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
        cout << "\r" << autoMove->peek() << " " << backTrackStack->peek();
        SetConsoleActiveScreenBuffer(hConsole);
    }



    if(player->fPlayerA == targetAngle) {

//        int peekBackTrack = backTrackStack->peek();

        int lastPop = autoMove->peek();


        // Move player forward a bit
        player->beingCoord->x += sinf(player->fPlayerA) * player->fSpeed * fElapsedTime * 0.6f;
        player->beingCoord->y += cosf(player->fPlayerA) * player->fSpeed * fElapsedTime * 0.6f;

        // Cast a ray directly in front, if it hits either a wall or a gate, pop off main stack and move player back to middle of tile
        if(castOneRay(0.5f, targetAngle, '#', false) || castOneRay(0.5f, targetAngle, 'I', false)) {
            autoMove->pop();
            player->beingCoord->x = floor(player->beingCoord->x) + 0.5f;
            player->beingCoord->y = floor(player->beingCoord->y) + 0.5f;
        } else {
            if(lastPop == 0) {  // East
                // If You're approximately on the next tile to the East, center the player on the tile
                if(approxEqual(player->beingCoord->y, player->prevBeingCoord->y + 1.0f, 0.3f)) {
                    player->beingCoord->y = floor(player->beingCoord->y) + 0.5f;
                    player->prevBeingCoord->y = floor(player->beingCoord->y) + 0.5f;

                    pushMoves = true;
                }
            } else if(lastPop == 1) { // South
                if(approxEqual(player->beingCoord->x, player->prevBeingCoord->x + 1.0f, 0.3f)) {
                    player->beingCoord->x = floor(player->beingCoord->x) + 0.5f; //double + float happening here
                    player->prevBeingCoord->x = floor(player->beingCoord->x) + 0.5f;

                    pushMoves = true;
                }
            } else if(lastPop == 2) { // West
                if(approxEqual(player->beingCoord->y, player->prevBeingCoord->y - 1.0f, 0.3f)) {
                    player->beingCoord->y = floor(player->beingCoord->y) + 0.5f;
                    player->prevBeingCoord->y = floor(player->beingCoord->y) + 0.5f;

                    pushMoves = true;
                }
            } else if(lastPop == 3) { // North
                if(approxEqual(player->beingCoord->x, player->prevBeingCoord->x - 1.0f, 0.3f)) {
                    player->beingCoord->x = floor(player->beingCoord->x) + 0.5f;
                    player->prevBeingCoord->x = floor(player->beingCoord->x) + 0.5f;

                    pushMoves = true;

//            } else if(currentLevel->levelMap->mapString.c_str()[((int)player->beingCoord->x - 1) * currentLevel->levelMap->getWidth() + (int)player->beingCoord->y] == '#') {
                }
            } else if(lastPop == 4) { // Should never reach here
                autoMove->push(backTrackStack->pop());
                player->fFOV += 3.0f;
                this_thread::sleep_for(chrono::milliseconds(500));
                player->fFOV -= 3.0f;
            } else if(lastPop == -1) {
                quit = true;
            } else if(lastPop == -2) {
                pushMoves = true;
            } else {
                pushMoves = true;
            }

            if(pushMoves) {
                lastPop = autoMove->pop();
                // Using breadcrumb method
                if(currentLevel->levelMap->mapString[(int)floor(player->beingCoord->x) * currentLevel->levelMap->getWidth() + (int)floor(player->beingCoord->y)] != '*') {
                    currentLevel->levelMap->mapString[(int)floor(player->beingCoord->x) * currentLevel->levelMap->getWidth() + (int)floor(player->beingCoord->y)] = '*';

                    // First push a backtracking move onto the main stack
                    autoMove->push(4);
                    // Push the direction of that backtracking move onto the backtrack stack
                    backTrackStack->push(lastPop);

                    // On the main stack, push the other moves that don’t represent backtracking, in a consistent order
                    // Try to push 3, 2 , 1, 0, but don't push the value if it represents Backtracking
                    for(int i = 3; i > -1; i--) {
                        if(i != (lastPop + 2) % 4)
                            autoMove->push(i);
                    }
                }
                this_thread::sleep_for(chrono::milliseconds(200));
            }

            // Tile the player is currently on
            player->onCurrentTile = currentLevel->levelMap->mapString[((int)player->beingCoord->x) * currentLevel->levelMap->getWidth() + (int)player->beingCoord->y];
            currentLevel->update(this);
        }

    } else if(player->fPlayerA != targetAngle) {
        // Gradually change player angle to the desired direction,
        // if approximately equal to the target angle,
        // set it equal to the target angle
        if (approxEqual(player->fPlayerA, targetAngle, 0.1f)) {
            player->fPlayerA = targetAngle;
            this_thread::sleep_for(chrono::milliseconds(200));
        } else if(player->fPlayerA < targetAngle) {
            player->fPlayerA += (player->fSpeed) * fElapsedTime;
            if(player->fPlayerA > targetAngle)
                player->fPlayerA = targetAngle;
        } else {
            player->fPlayerA -= (player->fSpeed) * fElapsedTime;
            if(player->fPlayerA < targetAngle)
                player->fPlayerA = targetAngle;
        }

        // Make sure player angle stays within 2pi radians
        player->fPlayerA = fmod(player->fPlayerA, (3.14159f * 2.0f));
    }

    // This should never happen, but,
    // If the player finds itself inside of a wall, move backwards until it ain't in a wall no mo.
    while(currentLevel->levelMap->mapString.c_str()[((int)player->beingCoord->x) * currentLevel->levelMap->getWidth() + (int)player->beingCoord->y] == '#') {
        player->beingCoord->x -= sinf(player->fPlayerA) * player->fSpeed * fElapsedTime;
        player->beingCoord->y -= cosf(player->fPlayerA) * player->fSpeed * fElapsedTime;
    }

    return;
}
