#ifndef GGAMEOVER_H
#define GGAMEOVER_H

#include "Game.h"

const TUint START_DELAY = FRAMES_PER_SECOND;

class GGameOverButtonWidget;
class GRetryButtonWidget;

static void center(const char *s, TInt y, TBool inverse = EFalse);

class GGameOver : public GDialogWidget {
public:
  EXPLICIT GGameOver(GGameState *aGameState);
  ~GGameOver() OVERRIDE;

public:
  void Run() OVERRIDE;

protected:
  GGameState *mGameState;
  TRGB mSavedPalette[256];
  TInt mState;
  TUint mTimer;
};

#endif
