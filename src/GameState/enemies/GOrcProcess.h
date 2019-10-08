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
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;
};

#endif //MODITE_GORCPROCESS_H
