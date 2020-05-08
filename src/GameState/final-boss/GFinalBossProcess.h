#ifndef GFINALBOSSPROCESS_H
#define GFINALBOSSPROCESS_H

#include "GLivingProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"

class GFinalBossProcess : public GLivingProcess {
public:
  GFinalBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TInt aIp, TInt16 aParams);
  ~GFinalBossProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

  void DeathAnimationDone(){
    mDeathCounter--;
  }

protected:
  void Idle(DIRECTION aDirection);
  void Walk(DIRECTION aDirection);
  void Projectile(DIRECTION aDirection);
  void Teleport(DIRECTION aDirection);
  void Hit(DIRECTION aDirection);
  void Spell(DIRECTION aDirection);
  void Death(DIRECTION aDirection);

protected:
  TBool MaybeAttack();
  TBool MaybeHit();

  TBool IdleState();
  TBool WalkState();
  TBool ProjectileState();
  TBool TeleportState();
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
  TBool mAttackType;
  TInt mHitTimer;
  void SetAttackTimer() { mAttackTimer = 5 * 60; }
};

#endif
