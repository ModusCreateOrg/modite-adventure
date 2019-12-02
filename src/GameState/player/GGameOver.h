#ifndef GGAMEOVER_H
#define GGAMEOVER_H

#include "Game.h"

class GGameOver {
public:
  GGameOver(GGameState *aGameState);
  ~GGameOver();

public:
  TBool Run();

protected:
  GGameState *mGameState;
  TRGB mSavedPalette[256];
  TInt mState;
};

#endif
