#ifndef MODITE_GTROLLPROCESS_H
#define MODITE_GTROLLPROCESS_H

#include <GEnemyProcess.h>
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

  TBool MaybeHit();

  TBool IdleState() override;

  TBool WalkState() override;

  TBool AttackState() override;

  TBool HitState() override;

  TBool DeathState() override;

protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GTROLLPROCESS_H
