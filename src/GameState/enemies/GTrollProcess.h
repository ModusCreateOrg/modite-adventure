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
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;
};

#endif // MODITE_GTROLLPROCESS_H
