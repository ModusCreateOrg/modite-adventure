#ifndef MODITE_GENEMYARROWPROCESS_H
#define MODITE_GENEMYARROWPROCESS_H

#include "GEnemyProjectileProcess.h"

struct SpriteData {
  TFloat x;
  TFloat y;
  TInt16 cx;
  TInt16 cy;
  TUint16 imageNumber;
  TUint32 flags;
  struct {
    TFloat x;
    TFloat y;
  } parry;
};

class GEnemyArrowProcess : public GEnemyProjectileProcess {
public:
  GEnemyArrowProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TUint16 aSlot, TUint16 aImg = 0);

public:
  void ConfigSprite(TFloat aAngle, TBool aParry = EFalse);

public:
  void Explode() OVERRIDE;
  void Parry() OVERRIDE;

protected:
  TInt16 mExplodeAnimation[11] = { 0 };
  TFloat mAngle;
};

#endif //MODITE_GENEMYARROWPROCESS_H
