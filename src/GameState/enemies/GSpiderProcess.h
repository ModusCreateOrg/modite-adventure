#ifndef MODITE_GSPIDERPROCESS_H
#define MODITE_GSPIDERPROCESS_H

#include "GEnemyProcess.h"
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GSpiderProcess : public GEnemyProcess {
public:
  GSpiderProcess(GGameState *aGameState,  TFloat aX, TFloat aY, TUint16 aParams);

  ~GSpiderProcess() OVERRIDE;

protected:
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;

  TBool CanWalk();

  TBool IdleState() OVERRIDE;

  TBool WalkState() OVERRIDE;

protected:
  TInt16 mStateTimer;
};

#endif //MODITE_GSPIDERPROCESS_H
