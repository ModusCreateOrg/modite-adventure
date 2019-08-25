//#define __DINGUX__
//
//
//
//#ifdef __DINGUX__

#include "LDKDisplay.h"
#include <iostream>
#include <SDL.h>

#include <cstdlib>

extern "C" {

int main(int argc, char *argv[]) {
  LDKDisplay *myDisplay = new LDKDisplay();
  myDisplay->Init();

  Uint32 lastAnimTime = SDL_GetTicks();

  bool loop = true;
  while (loop) {
    myDisplay->Draw();


    Uint32 currentTime = SDL_GetTicks();
    if (currentTime < lastAnimTime + 20) {
      SDL_Delay(lastAnimTime + 20 - currentTime);
    }

    lastAnimTime = SDL_GetTicks();
  }

  exit(0);

  return 0;
}

} // end extern "C":

//#endif