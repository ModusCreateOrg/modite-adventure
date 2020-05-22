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

GEnvironmentProcess::~GEnvironmentProcess() = default;
