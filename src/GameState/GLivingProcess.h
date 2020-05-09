#ifndef MODITE_GLIVINGPROCESS_H
#define MODITE_GLIVINGPROCESS_H

#include "GProcess.h"
#include "GResources.h"

class GLivingProcess : public GProcess {
public:
  EXPLICIT GLivingProcess(GGameState *aGameState, TUint16 aAttribute);
  ~GLivingProcess() OVERRIDE;

protected:
  void StartBlink(TUint16 aTime) {
    mBlinkTimer = aTime;
  }

private:
  virtual void BlinkOn() {
    if (mSprite) {
      mSprite->mFill = COLOR_WHITE;
    }
  };

  virtual void BlinkOff() {
    if (mSprite) {
      mSprite->mFill = -1;
    }
  };

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

public:
  TBool mInvulnerable;   // cannot be attacked

protected:
  GGameState *mGameState;

private:
  TUint16 mBlinkTimer;
};

#endif //MODITE_GLIVINGPROCESS_H
