#ifndef GFINALBOSSPROCESS_H
#define GFINALBOSSPROCESS_H

#include "GProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"

class GFinalBossProcess : public GProcess {
public:
  GFinalBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TInt aIp, TInt16 aParams);
  ~GFinalBossProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

  void DeathAnimationDone(){
    mDeathCounter--;
  }

protected:
  void Idle(DIRECTION aDirection);
  void Walk(DIRECTION aDirection);
  void Fire(DIRECTION aDirection);
  void Hit(DIRECTION aDirection);
  void Spell(DIRECTION aDirection);
  void Death(DIRECTION aDirection);

protected:
  TBool MaybeFire();
  TBool MaybeHit();

  TBool IdleState();
  TBool WalkState();
  TBool FireState();
  TBool HitState();
  TBool SpellState();
  TBool DeathState();
  void SetState(TInt aNewState, DIRECTION aNewDirection);

protected:
  GGameState *mGameState;
  DIRECTION mDirection;
  TInt mState;
  TInt mStep;
  TInt mDeathCounter;
  TInt mSpellCounter;

protected:
  TInt mStateTimer;
  TInt mAttackTimer;
  TInt mHitTimer;
  void SetAttackTimer() { mAttackTimer = 5 * 60; }
};

#endif
