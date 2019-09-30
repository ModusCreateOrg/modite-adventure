#ifndef MODITE_GTROLLPROCESS_H
#define MODITE_GTROLLPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GTrollProcess : public GEnemyProcess {
public:
  GTrollProcess(
      GGameState *aGameState, GGamePlayfield *aPlayfield, TFloat aX, TFloat aY);

  ~GTrollProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;

protected:
  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

  TBool HitState() OVERRIDE;

  TBool DeathState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif // MODITE_GTROLLPROCESS_H
