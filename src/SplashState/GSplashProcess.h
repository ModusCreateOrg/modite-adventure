#ifndef MODITE_GSPLASHPROCESS_H
#define MODITE_GSPLASHPROCESS_H

#include "Game.h"

class GSplashProcess : public BProcess {
public:
  GSplashProcess();
  ~GSplashProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

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
  BFont *mFont;
  const char *mCurrentText;
};

#endif //MODITE_GSPLASHPROCESS_H
