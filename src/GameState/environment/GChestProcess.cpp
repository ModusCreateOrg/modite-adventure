#include "GChestProcess.h"
#include "GGamePlayfield.h"
#include "GItemProcess.h"

GChestProcess::GChestProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY) {
  mGameState = aGameState;
  mParam = aParam;
  mSprite = new GAnchorSprite(999, ENVIRONMENT_SLOT, IMG_CHEST, STYPE_ENEMY);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->flags |= SFLAG_CHECK;
  mSprite->w = mSprite->h = 32;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY;
  mGameState->AddSprite(mSprite);
}

GChestProcess::~GChestProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GChestProcess::RunBefore() {
  return ETrue;
}

TBool GChestProcess::RunAfter() {
  if (mSprite->cType & STYPE_PBULLET) {
    mSprite->type = STYPE_DEFAULT;
    mSprite->cType = 0;
    mSprite->flags &= ~SFLAG_CHECK;
    mSprite->mImageNumber= IMG_CHEST + 1;   // chest open image
    printf("Chest open param = %x %d\n", mParam, mParam);
    GItemProcess::SpawnItem(mGameState, mParam, mSprite->x, mSprite->y);
  }
  return ETrue;
}
