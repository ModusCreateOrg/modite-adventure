#ifndef MODITE_GTITLEPROCESS_H
#define MODITE_GTITLEPROCESS_H

#include "Game.h"

class GTitleProcess : public BProcess {
public:
  GTitleProcess();
  virtual ~GTitleProcess();
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
  BFont *mFont;
  const char *mCurrentText;
};

#endif //MODITE_GTITLEPROCESS_H
