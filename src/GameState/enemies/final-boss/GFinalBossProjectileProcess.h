#ifndef GFINALBOSSPROJECTILEPROCESS_H
#define GFINALBOSSPROJECTILEPROCESS_H

#include "GGameState.h"
#include "GProcess.h"

class GFinalBossProjectileProcess : public GProcess {
public:
  GFinalBossProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, ELEMENT aElement);
  ~GFinalBossProjectileProcess() OVERRIDE;

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
