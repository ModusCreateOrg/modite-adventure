#ifndef GWIZARDPROJECTILEPROCESS_H
#define GWIZARDPROJECTILEPROCESS_H

#include "GWizardProcess.h"
#include "GGameState.h"

class GWizardProjectileProcess : public GProcess {
public:
  GWizardProjectileProcess(GGameState *aGameState, GWizardProcess *aParent, TInt aAngle);
  ~GWizardProjectileProcess();

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  TInt mStep;
  TInt mTimer;
};

#endif
