#ifndef GFINALBOSSDEATHPROCESS_H
#define GFINALBOSSDEATHPROCESS_H

#include "GFinalBossProcess.h"

class GFinalBossDeathProcess : public GProcess {
public:
  GFinalBossDeathProcess(GGameState *aGameState, GFinalBossProcess *aFinalBoss, TFloat aX, TFloat aY, TInt aDelay);
  ~GFinalBossDeathProcess() OVERRIDE;
public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;
protected:
  GGameState *mGameState;
  GFinalBossProcess *mFinalBoss;
  BAnimSprite *mSprite;
  TInt mTimer;
  TFloat mX, mY;
};

#endif
