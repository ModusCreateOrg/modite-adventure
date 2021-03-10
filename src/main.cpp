#include "Game.h"

BViewPort *gViewPort;
BGameEngine *gGameEngine;
TOptions *gOptions;
GGame *gGame;


#ifdef __MODUS_TARGET_NETWORK_DISPLAY__
#include "NetworkDisplay/NetworkDisplay.h"
#endif

// app_main
extern "C" void app_main(const char *file[]) {

    gGame = new GGame();
#ifdef __MODUS_TARGET_NETWORK_DISPLAY__
    NetworkDisplay *display = (NetworkDisplay*)&gDisplay;
    const char *fileName = "/home/jgarcia/modite-rgb.ini";
    display->ConfigureFromIniFile(fileName);
#endif
    gGame->Run();

    delete gGame;
}


int main(int argc, const char *file[]) {
    app_main(file);
    return 0;
}


