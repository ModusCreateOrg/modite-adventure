#ifndef MODITE_GSLIMEPROCESS_H
#define MODITE_GSLIMEPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GSlimeProcess : public GEnemyProcess {
public:
  GSlimeProcess(GGameState *aGameState,  TFloat aX, TFloat aY, TUint16 aParams);

  ~GSlimeProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;

  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GSLIMEPROCESS_H
