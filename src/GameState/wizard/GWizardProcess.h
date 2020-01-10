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

public:
  void WriteToStream(BMemoryStream &aStream);
  void ReadFromStream(BMemoryStream &aStream);

protected:
  void SetState(TInt aState, DIRECTION aDirection);
  void SetAttackTimer();

protected:
  TBool MaybeHit();
  TBool MaybeAttack();

  void Idle(DIRECTION aDirection);
  TBool IdleState();
  void Walk(DIRECTION aDirection);
  TBool WalkState();
  void Projectile(DIRECTION aDirection);
  TBool ProjectileState();
  void Teleport(DIRECTION aDirection);
  TBool TeleportState();

protected:
  GGameState *mGameState;
  TUint16 mSlot;
  TInt mIp;
  TInt mType;
  TInt16 aSpriteSheet;
  DIRECTION mDirection;
  TFloat mStartX, mStartY;

protected:
  TInt mState;
  TInt mStep;
  TInt mStateTimer;
  TInt mAttackTimer;
  TBool mAttackType;
};

#endif
