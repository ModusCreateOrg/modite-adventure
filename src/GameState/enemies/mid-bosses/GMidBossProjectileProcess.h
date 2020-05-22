#ifndef GMIDBOSSFIREBALLPROCESS_H
#define GMIDBOSSFIREBALLPROCESS_H

#include "GProcess.h"

class GGameState;
class ProjectileSprite;

class GMidBossProjectileProcess : public GProcess {
public:
  GMidBossProjectileProcess(GGameState *agameState, TFloat aX, TFloat aY);
  ~GMidBossProjectileProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  ProjectileSprite *mSprite;
  TBool mState;   // false = flying, true = exploding
};

#endif
