#ifndef MODITE_GSPLASHPROCESS_H
#define MODITE_GSPLASHPROCESS_H

#include "Game.h"

class GSplashProcess : public BProcess {
public:
  GSplashProcess();
  ~GSplashProcess();

public:
  TBool RunBefore();

  TBool RunAfter();

protected:
  void RenderText();
  TBool FadeInState();
  TBool FadeOutState();
  TBool WaitState();
protected:
  enum {
    STATE_FADEIN,
    STATE_FADEOUT,
    STATE_WAIT,
  } mState;
  TInt mTimer;
  TInt mColor;
  const char *mCurrentText;
};

#endif //MODITE_GSPLASHPROCESS_H
