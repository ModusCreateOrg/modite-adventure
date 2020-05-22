#ifndef GMIDBOSSDEATHPROCESS_H
#define GMIDBOSSDEATHPROCESS_H

#include "GWizardProcess.h"

class GWizardDeathProcess : public GProcess {
public:
  GWizardDeathProcess(GGameState *aGameState, GWizardProcess *aWizard, TFloat aX, TFloat aY, TInt aDelay);
  ~GWizardDeathProcess() OVERRIDE;
public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;
protected:
  GGameState *mGameState;
  GWizardProcess *mWizard;
  BAnimSprite *mSprite;
  TInt mTimer;
  TFloat mX, mY;
};

#endif
