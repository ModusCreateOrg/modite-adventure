#include "GChestProcess.h"
#include "GGamePlayfield.h"

GChestProcess::GChestProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY) {
  mGameState = aGameState;
  mParam = aParam;
  mSprite = new GAnchorSprite(999, ENVIRONMENT_SLOT, IMG_CHEST, STYPE_ENEMY);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->w = mSprite->h = 32;
  mSprite->x = aX;
  mSprite->y = aY;
  mGameState->AddSprite(mSprite);
}

GChestProcess::~GChestProcess() {
  //
  if (mSprite) {
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GChestProcess::RunBefore() {
  return ETrue;
}

TBool GChestProcess::RunAfter() {
  return ETrue;
}
