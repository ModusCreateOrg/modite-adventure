#ifndef MODITE_GORCPROCESS_H
#define MODITE_GORCPROCESS_H

#include <GameState/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GOrcProcess : public GEnemyProcess {
public:
  GOrcProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GOrcProcess();

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

#endif //MODITE_GORCPROCESS_H
