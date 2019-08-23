#define __DINGUX__



#ifdef __DINGUX__

#include "LDKDisplay.h"

int main(int argc, char** argv) {
  if (argc && argv);

  LDKDisplay *myDisplay = new LDKDisplay();

  Uint32 lastAnimTime = SDL_GetTicks();

  bool loop = true;
  while (loop) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime < lastAnimTime + 20) {
      SDL_Delay(lastAnimTime+20-currentTime);
    }
    lastAnimTime = SDL_GetTicks();
//    myDisplay->Draw();
  }

  exit(0);

  return 0;
}

#endif