#include "GLivingProcess.h"

GLivingProcess::GLivingProcess(GGameState *aGameState, TUint16 aAttribute) : GProcess(aAttribute), mGameState(aGameState) {
  mInvulnerable = EFalse;
  mBlinkTimer = 0;
  mStep = 0;
  mState = 0;
}

GLivingProcess::~GLivingProcess() = default;


TBool GLivingProcess::RunBefore() {
  return ETrue;
}

TBool GLivingProcess::RunAfter() {
  if (mBlinkTimer > 1) {
    mBlinkTimer--;
    if ((mBlinkTimer & 1u) == 0) {
      BlinkOn();
    } else {
      BlinkOff();
    }
  } else if (mBlinkTimer == 1) {
    mBlinkTimer--;
    BlinkOff();
    mInvulnerable = EFalse;
  }
  return ETrue;
}
