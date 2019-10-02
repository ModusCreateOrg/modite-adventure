#ifndef MODITE_GTROLLPROCESS_H
#define MODITE_GTROLLPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GTrollProcess : public GEnemyProcess {
public:
  GTrollProcess(GGameState *aGameState,  TFloat aX, TFloat aY, TUint16 aParams);

  ~GTrollProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) override;

protected:
  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif // MODITE_GTROLLPROCESS_H
