#ifndef GGAMEOVER_H
#define GGAMEOVER_H

#include "Game.h"

class GGameOver {
public:
  GGameOver();
  ~GGameOver();

public:
  TBool Run();

protected:
  TRGB mSavedPalette[256];
  TInt mState;
};

#endif
