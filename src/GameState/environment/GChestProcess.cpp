#include "GChestProcess.h"
#include "GPlayer.h"
#include "inventory/GItemProcess.h"

GChestProcess::GChestProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aIsOpen)
  : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY) {

  if (aIsOpen) {
    mSprite = new GAnchorSprite(aGameState, CHEST_PRIORITY, ENVIRONMENT_SLOT, IMG_CHEST + 1, STYPE_OBJECT);
    //    mSprite->SetCType(STYPE_OBJECT);
    //    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->type = STYPE_DEFAULT;
    mSprite->Name("ENVIRONMENT CHEST OPEN");
    mAttribute = ATTR_CHEST_OPEN;
  } else {
    mSprite = new GAnchorSprite(aGameState, CHEST_PRIORITY, ENVIRONMENT_SLOT, IMG_CHEST, STYPE_OBJECT);
    mSprite->Name("ITEM CHEST CLOSED");
    mAttribute = ATTR_CHEST;
  }
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_ENEMY);
  mSprite->w = 32;
  mSprite->h = 24;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 32;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
  mGameState->AddSprite(mSprite);

  if (aIsOpen) {
    GItemProcess::SpawnItem(mGameState, mIp, mParam, mSprite->x, mSprite->y);
  }
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
  if (mSprite->TestCType(STYPE_PBULLET)) {
//    mSprite->type = STYPE_DEFAULT;
//    mSprite->cType = 0;
//    mSprite->ClearFlags(SFLAG_CHECK);
    gSoundPlayer.TriggerSfx(SFX_CHEST_OPEN_WAV);
    mSprite->ClearCMask(STYPE_PBULLET);
    mSprite->ClearCType(STYPE_PBULLET);
    mSprite->mImageNumber = IMG_CHEST + 1; // chest open image
    printf("OPEN CHEST at %d,%d %x %d\n", TInt(mSprite->x), TInt(mSprite->y), mParam, mParam);
    mGameState->EndProgram(mIp, ATTR_CHEST_OPEN, mParam);
    GItemProcess::SpawnItem(mGameState, mIp, mParam, GPlayer::mSprite->x+32, GPlayer::mSprite->y);
    //0x5611b1dc89d0
  }
  mSprite->TestAndClearCType(STYPE_PLAYER | STYPE_ENEMY);

  return ETrue;
}
