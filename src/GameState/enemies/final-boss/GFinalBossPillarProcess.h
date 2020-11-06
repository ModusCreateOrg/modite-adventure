#ifndef GFINALBOSSPILLARPROCESS_H
#define GFINALBOSSPILLARPROCESS_H

#include "GProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"
#include "GFinalBossPillarSprite.h"

class GFinalBossPillarProcess : public GProcess {
public:
  GFinalBossPillarProcess(GGameState *aGameState, TFloat aX, TFloat aY, ELEMENT aElement, TBool aFollowPlayer, TInt aStartDelay);
  ~GFinalBossPillarProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  GGameState *mGameState;
  TBool mExploding;
  TInt16 mStartDelay;
  TBool mFollowPlayer;
  GFinalBossPillarSprite *mSprite;

};

#endif
