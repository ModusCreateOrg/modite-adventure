#ifndef MODITE_GBATPROCESS_H
#define MODITE_GBATPROCESS_H

#include <GameState/enemies/GEnemyProcess.h>
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GAnchorSprite.h"

class GBatProcess : public GEnemyProcess {
public:
  GBatProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams);

  ~GBatProcess() OVERRIDE;

protected:
  TBool CanWalk(DIRECTION aDirection, TFloat aVx, TFloat aVy) OVERRIDE;
  void NewState(TUint16 aState, DIRECTION aDirection) OVERRIDE;
  void Idle(DIRECTION aDirection) OVERRIDE;
  void Walk(DIRECTION aDirection) OVERRIDE;
  void Attack(DIRECTION aDirection) OVERRIDE;
  void Hit(DIRECTION aDirection) OVERRIDE;
  void Death(DIRECTION aDirection) OVERRIDE;
  void Land(DIRECTION aDirection);

protected:
  TFloat mAltitude;
  TInt mLandTimer;
};

#endif // MODITE_GBATPROCESS_H
