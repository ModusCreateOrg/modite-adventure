#ifndef MODITE_GSPIDERPROCESS_H
#define MODITE_GSPIDERPROCESS_H

#include <GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GSpiderProcess : public GEnemyProcess {
public:
  GSpiderProcess(GGameState *aGameState, GGamePlayfield *aPlayfield);

  ~GSpiderProcess();

public:
  TBool RunBefore();

  TBool RunAfter();

protected:
  void NewState(TUint16 aState, TUint16 aDirection);

  TBool IdleState();

  TBool WalkState();

  TBool AttackState();

  TBool HitState();
};

#endif //MODITE_GSPIDERPROCESS_H
