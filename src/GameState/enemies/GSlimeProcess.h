#ifndef MODITE_GSLIMEPROCESS_H
#define MODITE_GSLIMEPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GSlimeProcess : public GEnemyProcess {
public:
  GSlimeProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams);

  ~GSlimeProcess() OVERRIDE;

protected:
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Taunt(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;
};

#endif //MODITE_GSLIMEPROCESS_H
