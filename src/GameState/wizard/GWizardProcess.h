#ifndef GWIZARDPROCESS_H
#define GWIZARDPROCESS_H

#include "GAnchorSprite.h"
#include "GProcess.h"
#include "GGameState.h"
#include "common/GSpellOverlayProcess.h"

enum {
  STATE_IDLE,
  STATE_WALK,
  STATE_PROJECTILE,
  STATE_TELEPORT,
  STATE_ILLUSION,
  STATE_SPELL,
  STATE_DEATH,
};

class GWizardProcess : public GProcess {
public:
  GWizardProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TInt aIp, TInt aType, TUint16 aAttribute, TUint16 aSpriteSheet);
  ~GWizardProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

  TBool IsChanneling() { // wizard is sustaining active spell (keep associated processes alive)
    return mChanneling;
  }

  void DeathAnimationDone(){
    mDeathCounter--;
  }

public:
  void WriteToStream(BMemoryStream &aStream);
  void ReadFromStream(BMemoryStream &aStream);

protected:
  void SetState(TInt aState, DIRECTION aDirection);
  void SetAttackTimer();
  void RandomLocation();

protected:
  TBool MaybeDamage();
  TBool MaybeDeath();
  TBool MaybeAttack();

  void Idle(DIRECTION aDirection);
  TBool IdleState();
  void Walk(DIRECTION aDirection);
  TBool WalkState();
  void Projectile();
  TBool ProjectileState();
  void Teleport();
  TBool TeleportState();
  void Illusion();
  TBool IllusionState();
  void Death();
  TBool DeathState();

protected:
  GGameState *mGameState;
  TUint16 mSlot;
  TInt mIp;
  TInt mType;
  TInt16 mSpriteSheet;
  DIRECTION mDirection;
  TFloat mStartX, mStartY;

protected:
  TInt mState;
  TInt mStep;
  TInt mAttackType;
  TInt mDeathCounter, mSpellCounter;
  TInt mStateTimer, mAttackTimer, mHitTimer, mBlinkTimer;
  TBool mChanneling;
};

#endif
