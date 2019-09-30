#ifndef MODITE_GSLIMEPROCESS_H
#define MODITE_GSLIMEPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GSlimeProcess : public GEnemyProcess {
public:
  GSlimeProcess(GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GSlimeProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;

  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

  TBool HitState() OVERRIDE;

  TBool DeathState() OVERRIDE;
protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GSLIMEPROCESS_H
