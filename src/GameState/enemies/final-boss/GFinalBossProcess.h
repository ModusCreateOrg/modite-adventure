#ifndef GFINALBOSSPROCESS_H
#define GFINALBOSSPROCESS_H

#include "GBossProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"
#include "GPlayerProcess.h"

class GFinalBossProcess : public GBossProcess {
public:
  GFinalBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TInt aIp, TInt16 aParams);
  ~GFinalBossProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  void SetAttackTimer();

protected:
  void Idle(DIRECTION aDirection);
  void Walk(DIRECTION aDirection);
  void Charge(DIRECTION aDirection);
  void Leap(DIRECTION aDirection);
  void Land(DIRECTION aDirection);
  void Projectile(DIRECTION aDirection);
  void Spell(DIRECTION aDirection);
  void Death(DIRECTION aDirection);

protected:
  TBool MaybeAttack();
  TBool MaybeHit();
  TBool MaybeBounce();

  TBool InitializeState();
  TBool IdleState();
  TBool WalkState();
  TBool ChargeState();
  TBool LeapState();
  TBool ProjectileState();
  TBool SpellState();
  TBool DeathState();
  void SetState(TInt aNewState, DIRECTION aNewDirection);

protected:
  TBool mAttackType;
};

#endif
