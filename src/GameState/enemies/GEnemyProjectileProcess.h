#ifndef GENEMYPROJECTILEPROCESS_H
#define GENEMYPROJECTILEPROCESS_H

#include "GProcess.h"
#include "GGameState.h"

class GEnemyProjectileProcess : public GProcess {
public:
  GEnemyProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TUint16 aSlot, TUint16 aImg = 0);
  ~GEnemyProjectileProcess();

public:
  void ConfigSprite(TFloat aAngle, TBool aParry = EFalse);
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  TInt mTimer;
  TFloat mAngle;
};

#endif
