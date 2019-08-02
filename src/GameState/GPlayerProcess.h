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
  void NewState(TUint16 aState, TUint16 aDirection);
  TBool MaybeWalk();
  TBool MaybeSword();
  TBool IdleState();
  TBool WalkState();
  TBool SwordState();
protected:
  GGameState *mGameState;
  GGamePlayfield *mPlayfield;
  BAnimSprite *mSprite;
  TUint16 mState;
  TUint16 mDirection;
  TUint16 mStep;
};


#endif //MODITE_GPLAYERPROCESS_H
