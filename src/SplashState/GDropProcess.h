#ifndef MODITE_GDROPPROCESS_H
#define MODITE_GDROPPROCESS_H

#include "Game.h"

class GDropProcess : public BProcess {
public:
  GDropProcess(GSplashState *aSplashState, TInt16 aDropNumber);
protected:
  void SetTimer();
  TBool WaitState();
  TBool FormState();
  TBool FallState();
  TBool RunBefore() {
    return ETrue;
  }
  TBool RunAfter();

protected:
  const TFloat GRAVITY = .08f;
  BAnimSprite *mSprite;
  GSplashState *mSplashState;
  TInt16 mTimer;
  TInt16 mDropNumber;
  enum {
    STATE_WAIT,
    STATE_FORM,
    STATE_FALL
  } mState;
};

#endif //MODITE_GDROPPROCESS_H
