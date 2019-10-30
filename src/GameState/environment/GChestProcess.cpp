#include "GChestProcess.h"
#include "inventory/GItemProcess.h"

GChestProcess::GChestProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aIsOpen)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY) {

  if (aIsOpen) {
    mSprite = new GAnchorSprite(aGameState, CHEST_PRIORITY, ENVIRONMENT_SLOT, IMG_CHEST + 1, STYPE_ENEMY);
    mSprite->cType = 0;
    mSprite->ClearFlags(SFLAG_CHECK | SFLAG_ANCHOR);
    mSprite->type = STYPE_DEFAULT;

    mSprite->y = aY;
  }
  else {
    mSprite = new GAnchorSprite(aGameState, CHEST_PRIORITY, ENVIRONMENT_SLOT, IMG_CHEST, STYPE_ENEMY);
    mSprite->SetCMask(STYPE_PBULLET | STYPE_PLAYER);
    mSprite->cx = -16;
    mSprite->y = aY + 32;

  }

  mSprite->w = 32;
  mSprite->h = 24;
  mSprite->x = aX;

  mGameState->AddSprite(mSprite);

  if (aIsOpen) {
    GItemProcess::SpawnItem(mGameState, mIp, mParam, mSprite->x, mSprite->y+32);
  }
}

GChestProcess::~GChestProcess() {
  //
}

TBool GChestProcess::RunBefore() {
  return ETrue;
}

TBool GChestProcess::RunAfter() {
  if (mSprite->TestCType(STYPE_PBULLET)) {
    mSprite->type = STYPE_DEFAULT;
    mSprite->cType = 0;
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->mImageNumber = IMG_CHEST + 1; // chest open image
    printf("Chest open param = %x %d\n", mParam, mParam);
    GItemProcess::SpawnItem(mGameState, mIp, mParam, mSprite->x, mSprite->y+32);
    //0x5611b1dc89d0
    mGameState->EndProgram(mIp, ATTR_CHEST_OPEN, mParam);
  }

  return ETrue;
}
