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
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;
};

#endif // MODITE_GBATPROCESS_H
