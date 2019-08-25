#ifndef MODITE_LDKDISPLAY_H
#define MODITE_LDKDISPLAY_H


#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>




class LDKDisplay {
public:
  LDKDisplay() {

  }

  void Init() {
    SDL_ShowCursor(SDL_DISABLE);

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == -1) {
      printf("Could not load SDL : %s\n", SDL_GetError());
      exit(-1);
    }

    atexit(SDL_Quit);



    mSDLScreen = SDL_SetVideoMode(
      320,
      240,
      16,
      SDL_HWSURFACE |

      #ifdef SDL_TRIPLEBUF
        SDL_TRIPLEBUF
      #else
        SDL_DOUBLEBUF
      #endif
    );

    mDrawSurface = SDL_CreateRGBSurface(
      SDL_HWSURFACE,
      320,
      240,
      16,
      0,
      0,
      0,
      0
    );

  }
//  uint32_t color = 0;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;

  void Draw() {

    SDL_FillRect(mDrawSurface, &mSDLScreen->clip_rect, SDL_MapRGB(mSDLScreen->format, r++, b=b+2, g=g+3));
//
    SDL_Rect destination;
    destination.x = 0;
    destination.y = 0;

    if (SDL_MUSTLOCK(mSDLScreen)) SDL_UnlockSurface(mSDLScreen);
    SDL_BlitSurface(mDrawSurface, &mDrawSurface->clip_rect, mSDLScreen, &destination);
    SDL_Flip(mSDLScreen);
    if (SDL_MUSTLOCK(mSDLScreen)) SDL_LockSurface(mSDLScreen);



  }

  ~LDKDisplay() {
    SDL_FreeSurface(mSDLScreen);
    SDL_Quit();
  }

  SDL_Surface *mDrawSurface;
  SDL_Surface *mSDLScreen;

};



#include <iostream>
#include <ctime>
#include <cstdlib>


#endif //MODITE_LDKDISPLAY_H
