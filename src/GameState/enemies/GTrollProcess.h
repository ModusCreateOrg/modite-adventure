#ifndef MODITE_GTROLLPROCESS_H
#define MODITE_GTROLLPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GTrollProcess : public GEnemyProcess {
public:
  GTrollProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GTrollProcess();

public:
  TBool RunBefore() override;

  TBool RunAfter() override;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;

protected:
  TBool MaybeHit();

  TBool MaybeAttack();

protected:
  TBool IdleState() override;

  TBool WalkState() override;

  TBool AttackState() override;

  TBool HitState() override;

  TBool DeathState() override;

protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GTROLLPROCESS_H
