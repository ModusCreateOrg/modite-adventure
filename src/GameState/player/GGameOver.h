#ifndef GGAMEOVER_H
#define GGAMEOVER_H

#include "Game.h"

const TUint START_DELAY = FRAMES_PER_SECOND;

class GGameOver {
public:
  EXPLICIT GGameOver(GGameState *aGameState);
  ~GGameOver();

public:
  TBool Run();

protected:
  GGameState *mGameState;
  TRGB mSavedPalette[256];
  TInt mState;
  TUint mTimer;
};

#endif
