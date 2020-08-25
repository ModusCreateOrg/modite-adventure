#ifndef MODITE_GGOBLINPROCESS_H
#define MODITE_GGOBLINPROCESS_H

#include <GameState/enemies/GGruntProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GGoblinProcess : public GGruntProcess {
public:
  GGoblinProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams);

  ~GGoblinProcess() OVERRIDE;

protected:
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Taunt(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  TBool WalkState() OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Spell(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;


};

#endif //MODITE_GGOBLINPROCESS_H
