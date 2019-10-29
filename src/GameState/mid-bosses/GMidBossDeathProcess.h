#ifndef GMIDBOSSDEATHPROCESS_H
#define GMIDBOSSDEATHPROCESS_H

//#include <BProcess.h>
//#include <BAnimSpite.h>
#include "GMidBossProcess.h"

//class GGameState;

class GMidBossDeathProcess : public BProcess {
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
