#ifndef MODITE_GGOBLINPROCESS_H
#define MODITE_GGOBLINPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GGoblinProcess : public GEnemyProcess {
public:
  GGoblinProcess(GGameState *aGameState,  TFloat aX, TFloat aY, TUint16 aParams);

  ~GGoblinProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;

  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GGOBLINPROCESS_H
