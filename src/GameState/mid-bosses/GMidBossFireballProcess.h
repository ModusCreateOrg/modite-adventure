#ifndef GMIDBOSSFIREBALLPROCESS_H
#define GMIDBOSSFIREBALLPROCESS_H

class GGameState;
class FireballSprite;

class GMidBossFireballProcess : public BProcess {
public:
  GMidBossFireballProcess(GGameState *agameState, TFloat aX, TFloat aY);
  ~GMidBossFireballProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  FireballSprite *mSprite;
  TBool mState;   // false = flying, true = exploding
};

#endif
