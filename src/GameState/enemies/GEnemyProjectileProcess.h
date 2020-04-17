#ifndef GENEMYPROJECTILEPROCESS_H
#define GENEMYPROJECTILEPROCESS_H

#include "GProcess.h"

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

class GEnemyProjectileProcess : public GProcess {
public:
  GEnemyProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TUint16 aSlot, TUint16 aImg = 0);
  ~GEnemyProjectileProcess();

public:
  void ConfigSprite(TFloat aAngle, TBool aParry = EFalse);
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  TInt mTimer;
  TFloat mAngle;
  TInt16 mExplodeAnimation[11] = { 0 };
  enum {
    LIVE,
    DIE,
  } mState;
};

#endif
