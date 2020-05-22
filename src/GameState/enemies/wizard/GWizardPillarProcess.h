#ifndef GWIZARDPILLARPROCESS_H
#define GWIZARDPILLARPROCESS_H

#include "GProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"
#include "GWizardProcess.h"

class GWizardPillarProcess : public GProcess {
public:
  GWizardPillarProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aAngle, TFloat aDistance, TBool aFollowPlayer, TUint16 aMaxDuration = UINT16_MAX);
  ~GWizardPillarProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  GGameState *mGameState;
  TBool mExploding;
  TUint16 mFrame;
  TUint16 mMaxDuration;
  TBool mFollowPlayer;
  GWizardProcess *mParent;

};

#endif
