#ifndef GWIZARDPROJECTILEPROCESS_H
#define GWIZARDPROJECTILEPROCESS_H

#include "GWizardProcess.h"
#include "GGameState.h"

class GWizardProjectileProcess : public GProcess {
public:
  GWizardProjectileProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aX, TFloat aY, TFloat aAngle, TInt16 aAttribute);
  ~GWizardProjectileProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  void StartProjectileAnimation();
  void StartExplodeAnimation();

protected:
  TInt mStep;
  TInt mTimer;
  TFloat mAngle;
  TInt16 mAttribute;
  GWizardProcess *mParent;
};

#endif
