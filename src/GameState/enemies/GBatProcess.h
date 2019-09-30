#ifndef MODITE_GBATPROCESS_H
#define MODITE_GBATPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GBatProcess : public GEnemyProcess {
public:
  GBatProcess(
      GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GBatProcess();

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;

  TBool IdleState() override;

  TBool CanWalk(TInt aDirection);

  TBool WalkState() override;

  TBool DeathState() override;

protected:
  TInt16 mStateTimer;
};

#endif // MODITE_GBATPROCESS_H
