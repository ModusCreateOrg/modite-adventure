#ifndef MODITE_GRATPROCESS_H
#define MODITE_GRATPROCESS_H

#include <GameState/enemies/GGruntProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GRatProcess : public GGruntProcess {
public:
  GRatProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams);

  ~GRatProcess() OVERRIDE;

protected:
  TBool CanWalkInDirection(DIRECTION aDirection, TFloat aVx, TFloat avY) OVERRIDE;

protected:
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Taunt(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Spell(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;
};

#endif // MODITE_GRATPROCESS_H
