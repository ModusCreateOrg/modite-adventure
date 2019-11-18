#ifndef GMIDBOSSDEATHPROCESS_H
#define GMIDBOSSDEATHPROCESS_H

#include "GMidBossProcess.h"

class GMidBossDeathProcess : public GProcess {
public:
  GMidBossDeathProcess(GGameState *aGameState, GMidBossProcess *aMidBoss, TFloat aX, TFloat aY, TInt aDelay);
  ~GMidBossDeathProcess() OVERRIDE;
public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;
protected:
  GGameState *mGameState;
  GMidBossProcess *mMidBoss;
  BAnimSprite *mSprite;
  TInt mTimer;
  TFloat mX, mY;
};

#endif
