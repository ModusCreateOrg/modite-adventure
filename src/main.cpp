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


int main() {
    app_main();
    return 0;
}


