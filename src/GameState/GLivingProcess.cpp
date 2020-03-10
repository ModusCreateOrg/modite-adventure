#include "GLivingProcess.h"

GLivingProcess::GLivingProcess(TUint16 aAttribute) : GProcess(aAttribute) {
  mInvulnerable = EFalse;
  mBlinkTimer = 0;
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
