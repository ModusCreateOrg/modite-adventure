#ifndef GENEMYCRYSTALPROCESS_H
#define GENEMYCRYSTALPROCESS_H

#include "GGameState.h"
#include "GEnemyProjectileProcess.h"

class GEnemyCrystalProcess : public GEnemyProjectileProcess {
public:
  GEnemyCrystalProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TUint16 aSlot, TUint16 aImg = 0);

public:
  void Explode() OVERRIDE;
  void Parry() OVERRIDE;

protected:
  TFloat mAngle;
};

#endif
