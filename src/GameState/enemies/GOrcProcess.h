#ifndef MODITE_GORCPROCESS_H
#define MODITE_GORCPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GOrcProcess : public GEnemyProcess {
public:
  GOrcProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aParams);

  ~GOrcProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;

  TBool CanWalk(DIRECTION aDirection);

  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GORCPROCESS_H
