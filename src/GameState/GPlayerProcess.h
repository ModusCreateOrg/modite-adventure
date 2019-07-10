#ifndef MODITE_GPLAYERPROCESS_H
#define MODITE_GPLAYERPROCESS_H

#include "Game.h"
class GGamePlayfield;

class GPlayerProcess : public BProcess {
public:
  GPlayerProcess(GGameState *aGameState, GGamePlayfield *aPlayfield);
  ~GPlayerProcess();
public:
  TBool RunBefore();
  TBool RunAfter();
protected:
  TBool IdleState();
  TBool WalkState();
protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
  BAnimSprite *mSprite;
};


#endif //MODITE_GPLAYERPROCESS_H
