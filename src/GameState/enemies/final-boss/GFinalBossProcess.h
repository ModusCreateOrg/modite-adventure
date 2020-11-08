#ifndef GFINALBOSSPROCESS_H
#define GFINALBOSSPROCESS_H

#include "GBossProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"
#include "GPlayerProcess.h"
#include "GFinalBossShieldProcess.h"

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
  void Projectile();
  void Spell(DIRECTION aDirection);
  void Death(DIRECTION aDirection);
  ELEMENT RandomElement();

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
  TBool PillarState();
  TBool StunState();
  TBool ResetShieldState();
  TBool DeathState();
  void SetState(TInt aNewState, DIRECTION aNewDirection);

  void RaiseShield();
  void LowerShield();

protected:
  TInt mAttackIndex;
  GFinalBossShieldProcess *mShieldProcess;
};

#endif
