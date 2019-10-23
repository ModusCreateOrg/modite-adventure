#include "GEnvironmentProcess.h"

GEnvironmentProcess::GEnvironmentProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TInt aPri)
    : BProcess(aPri) {
  mGameState = aGameState;
  mIp = aIp;
  mParam = aParam;
  mAttribute = (OBJECT_ATTRIBUTE *)&mParam;
  mSprite = mSprite1 = mSprite2 = ENull;
}

GEnvironmentProcess::~GEnvironmentProcess() {
  if (mSprite2) {
    mSprite2->Remove();
    delete mSprite2;
    mSprite2= ENull;
  }
  if (mSprite1) {
    mSprite1->Remove();
    delete mSprite1;
    mSprite1 = ENull;
  }
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}
