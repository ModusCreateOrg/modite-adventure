#ifndef GMIDBOSSFIREBALLPROCESS_H
#define GMIDBOSSFIREBALLPROCESS_H

class GGameState;
class FireballSprite;

class GMidBossProjectileProcess : public BProcess {
public:
  GMidBossProjectileProcess(GGameState *agameState, TFloat aX, TFloat aY);
  ~GMidBossProjectileProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  FireballSprite *mSprite;
  TBool mState;   // false = flying, true = exploding
};

#endif
