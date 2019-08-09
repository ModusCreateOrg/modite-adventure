//
// Created by mschwartz on 7/3/19.
//

#include "Game.h"

BViewPort *gViewPort;
BGameEngine *gGameEngine;
TOptions *gOptions;
GGame *gGame;

// app_main
extern "C" void app_main() {
    gGame = new GGame();
    gGame->Run();
    delete gGame;
}

#ifndef __XTENSA

int main() {
    app_main();
    return 0;
}

#endif

