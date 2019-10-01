#ifndef MODITE_GBATPROCESS_H
#define MODITE_GBATPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GBatProcess : public GEnemyProcess {
public:
  GBatProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aParams);

  ~GBatProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;

  TBool IdleState() OVERRIDE;

  TBool CanWalk(TInt aDirection);

  TBool WalkState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif // MODITE_GBATPROCESS_H
