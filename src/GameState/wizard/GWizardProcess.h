#ifndef GWIZARDPROCESS_H
#define GWIZARDPROCESS_H

#include "GAnchorSprite.h"
#include "GProcess.h"
#include "GGameState.h"

class GWizardProcess : public GProcess {
public:
  GWizardProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TInt aIp, TInt aType, TUint16 aAttribute, TUint16 aSpriteSheet);
  ~GWizardProcess();

public:
  TBool RunBefore();
  TBool RunAfter();
  void WriteToStream(BMemoryStream &aStream);
  void ReadFromStream(BMemoryStream &aStream);
public:
  void SetState(TInt aState, DIRECTION aDirection);

protected:
  GGameState *mGameState;
  TUint16 mSlot;
  TInt mIp;
  TInt mType;
  TInt16 aSpriteSheet;
  DIRECTION mDirection;

protected:
  TInt mState;
  TInt mStep;
  TInt mStateTimer;
  TInt mAttackTimer;
};

#endif
