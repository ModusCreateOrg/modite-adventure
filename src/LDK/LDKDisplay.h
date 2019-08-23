#ifndef MODITE_LDKDISPLAY_H
#define MODITE_LDKDISPLAY_H


#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>

class LDKDisplay {
public:
  LDKDisplay() {

  }

  void Init() {
    if(SDL_Init(SDL_INIT_VIDEO) == -1) {
      printf("Could not load SDL : %s\n", SDL_GetError());
      exit(-1);
    }

    atexit(SDL_Quit);

    SDL_ShowCursor(SDL_DISABLE);


    mSDLSurface = SDL_SetVideoMode(
      320,
      240,
      16,
      SDL_HWSURFACE |SDL_DOUBLEBUF
    );

    mDrawSurface = SDL_CreateRGBSurface(
      SDL_HWSURFACE,
      320,
      240,
      32,
      0,
      0,
      0,
      0
    );

  }
  uint32_t color = 0;


  void Draw() {

    SDL_FillRect(mSDLSurface, NULL, color++);

    SDL_Flip(mSDLSurface);



  }

  ~LDKDisplay() {
    SDL_FreeSurface(mSDLSurface);
    SDL_Quit();
  }

  SDL_Surface *mDrawSurface;
  SDL_Surface *mSDLSurface;

};



#include <iostream>
#include <ctime>
#include <cstdlib>


#endif //MODITE_LDKDISPLAY_H
