#ifndef GENEMYPROJECTILEPROCESS_H
#define GENEMYPROJECTILEPROCESS_H

#include "GProcess.h"

class GEnemyProjectileProcess : public GProcess {
public:
  GEnemyProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aImg = 0);
  ~GEnemyProjectileProcess() OVERRIDE;

public:
  virtual void Explode() = 0;
  virtual void Parry() = 0;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  TInt mTimer;
  enum {
    LIVE,
    DIE,
  } mState;
};

#endif
