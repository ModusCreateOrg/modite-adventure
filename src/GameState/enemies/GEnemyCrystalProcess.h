#ifndef GENEMYCRYSTALPROCESS_H
#define GENEMYCRYSTALPROCESS_H

#include "GGameState.h"
#include "GProcess.h"

class GEnemyCrystalProcess : public GProcess {
public:
  GEnemyCrystalProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TUint16 aSlot, TUint16 aImg = 0);
  ~GEnemyCrystalProcess();

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  TInt mTimer;
  TFloat mAngle;
  enum {
    LIVE,
    DIE,
  } mState;
};

#endif
