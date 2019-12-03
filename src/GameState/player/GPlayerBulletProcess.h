#ifndef MODITE_GPLAYERBULLETPROCESS_H
#define MODITE_GPLAYERBULLETPROCESS_H


#include <GameState/GProcess.h>

class GGameState;
class BulletSprite;

class GPlayerBulletProcess : public GProcess {
public:
  GPlayerBulletProcess(GGameState *aGameState, DIRECTION aDirection);
  ~GPlayerBulletProcess();

public:
  TBool RunBefore();
  TBool RunAfter();

protected:
  GGameState *mGameState;
  BulletSprite *mSprite;
  TFloat mAge;
  TFloat mRange;
  DIRECTION mDirection;
};


#endif //MODITE_GPLAYERBULLETPROCESS_H
