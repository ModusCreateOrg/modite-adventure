#ifndef GFINALBOSSPILLARPROCESS_H
#define GFINALBOSSPILLARPROCESS_H

#include "GProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"

class GFinalBossPillarProcess : public GProcess {
public:
  GFinalBossPillarProcess(GGameState *aGameState, TFloat aX, TFloat aY, TInt16 aSlot, TBool aFollowPlayer, TInt aStartDelay);
  ~GFinalBossPillarProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  TBool mExploding;
  TUint16 mFrame;
  TUint16 mStartDelay;
  TBool mFollowPlayer;

};

#endif
