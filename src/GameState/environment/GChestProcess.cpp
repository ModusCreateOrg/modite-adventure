#include "GChestProcess.h"
#include "inventory/GItemProcess.h"

GChestProcess::GChestProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY) {
  mSprite = new GAnchorSprite(aGameState, CHEST_PRIORITY, ENVIRONMENT_SLOT, IMG_CHEST, STYPE_ENEMY);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->w = mSprite->h = 32;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 32;
  mGameState->AddSprite(mSprite);
}

GChestProcess::~GChestProcess() {
  //
}

TBool GChestProcess::RunBefore() {
  return ETrue;
}

TBool GChestProcess::RunAfter() {
  if (mSprite->cType & STYPE_PBULLET) {
    mSprite->type = STYPE_DEFAULT;
    mSprite->cType = 0;
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->mImageNumber = IMG_CHEST + 1; // chest open image
    printf("Chest open param = %x %d\n", mParam, mParam);
    GItemProcess::SpawnItem(mGameState, mIp, mParam, mSprite->x, mSprite->y);
  }

  return ETrue;
}
