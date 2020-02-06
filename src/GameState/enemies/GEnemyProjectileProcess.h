#ifndef GENEMYPROJECTILEPROCESS_H
#define GENEMYPROJECTILEPROCESS_H

// #include "GEnemyProcess.h"
#include "GGameState.h"

class GEnemyProjectileProcess : public GProcess {
public:
  GEnemyProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle);
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
