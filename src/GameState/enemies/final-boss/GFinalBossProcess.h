#ifndef GFINALBOSSPROCESS_H
#define GFINALBOSSPROCESS_H

#include "GBossProcess.h"
#include "GAnchorSprite.h"
#include "GGameState.h"

class GFinalBossProcess : public GBossProcess {
public:
  GFinalBossProcess(GGameState *aGameState, TFloat aX, TFloat aY, TInt aIp, TInt16 aParams);
  ~GFinalBossProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

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
  TBool mAttackType;
  void SetAttackTimer() { mAttackTimer = 5 * 60; }
};

#endif
