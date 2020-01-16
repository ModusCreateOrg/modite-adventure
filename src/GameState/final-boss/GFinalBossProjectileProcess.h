#ifndef GFINALBOSSPROJECTILEPROCESS_H
#define GFINALBOSSPROJECTILEPROCESS_H

#include "GGameState.h"

class GFinalBossProjectileProcess : public GProcess {
public:
  GFinalBossProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TInt16 aAttribute);
  ~GFinalBossProjectileProcess();

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  void StartProjectileAnimation();
  void StartExplodeAnimation();

protected:
  TInt mStep;
  TInt mTimer;
  TInt16 mAttribute;
  TBool mExploding;
};

#endif
