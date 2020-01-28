#ifndef GWIZARDPILLARPROCESS_H
#define GWIZARDPILLARPROCESS_H

#include "GProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"
#include "GWizardProcess.h"

class GWizardPillarProcess : public GProcess {
public:
  GWizardPillarProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aX, TFloat aY, TBool aFollowPlayer, TInt aStartDelay);
  ~GWizardPillarProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  TBool mExploding;
  TUint16 mFrame;
  TUint16 mStartDelay;
  TBool mFollowPlayer;
  GWizardProcess *mParent;

};

#endif
