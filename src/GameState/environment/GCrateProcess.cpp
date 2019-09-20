#include "GCrateProcess.h"
#include "GGamePlayfield.h"

GCrateProcess::GCrateProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY) {
  mGameState = aGameState;
  mParam = aParam;
  mSprite = new GAnchorSprite(999, ENVIRONMENT_SLOT, IMG_CRATE, STYPE_ENEMY);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->w = mSprite->h = 32;
  mSprite->x = aX;
  mSprite->y = aY;
  mGameState->AddSprite(mSprite);
}

GCrateProcess::~GCrateProcess() {
  //
  if (mSprite) {
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GCrateProcess::RunBefore() {
  return ETrue;
}

TBool GCrateProcess::RunAfter() {
  return ETrue;
}
