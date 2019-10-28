#ifndef MODITE_GRATPROCESS_H
#define MODITE_GRATPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

const TInt RAT_RECT_X = 0;
const TInt RAT_RECT_Y = 0;
const TInt RAT_RECT_W = 32;
const TInt RAT_RECT_H = 20;

class GRatProcess : public GEnemyProcess {
public:
  GRatProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams);

  ~GRatProcess() OVERRIDE;

protected:
  TBool CanWalk(DIRECTION aDirection, TFloat aVx, TFloat avY) OVERRIDE;

protected:
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;
};

#endif // MODITE_GRATPROCESS_H
