#include "GEnvironmentProcess.h"

GEnvironmentProcess::GEnvironmentProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TInt aPri)
    : GProcess(0, aPri) {
  mGameState = aGameState;
  mSaveToStream = EFalse;
  mIp = aIp;
  mParam = aParam;
  mObjectAttribute = (OBJECT_ATTRIBUTE *)&mParam;
  mSprite = mSprite2 = ENull;
}

GEnvironmentProcess::~GEnvironmentProcess() {
  if (mSprite2) {
    mSprite2->Remove();
    delete mSprite2;
    mSprite2= ENull;
  }
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}
