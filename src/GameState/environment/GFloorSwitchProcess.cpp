#include "GFloorSwitchProcess.h"

const TInt ANIM_SPEED = FRAMES_PER_SECOND;

GFloorSwitchProcess::GFloorSwitchProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aWooden)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY) {
  mSprite = ENull;
  mImage = IMG_FLOOR_SWITCH + (aWooden ? 2 : 0);

  mSprite = new GAnchorSprite(mGameState, FLOOR_SWITCH_PRIORITY, ENVIRONMENT_SLOT, mImage, STYPE_OBJECT);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->w = mSprite->h = 32;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 3;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(DUNGEON_TILESET_OBJECTS_BMP);
  mGameState->AddSprite(mSprite);
  mState = mAnimating = EFalse;
}

GFloorSwitchProcess::~GFloorSwitchProcess() {
  //
}

TBool GFloorSwitchProcess::RunBefore() {
  GGamePlayfield *p = mGameState->mGamePlayfield;

  TInt group = mAttribute->group;
  if (group) {
    if (mState) {
      if (mAttribute->order != OA_ORDER_ANY) {
        p->mGroupState[group] = EFalse;
      }
    }
    else {
      p->mGroupState[group] = EFalse;
      p->mGroupDone[group] = EFalse;
    }
  }

  return ETrue;
}

TBool GFloorSwitchProcess::RunAfter() {
  if (mAnimating) {
    if (mSprite->AnimDone()) {
      mSprite->cType = 0;
      mSprite->SetFlags(SFLAG_CHECK);
      return ETrue;
    }
  }

  if (mSprite->TestAndClearCType(STYPE_PBULLET)) {
    mSprite->ClearFlags(SFLAG_CHECK);
    mState = !mState;
    OBJECT_ATTRIBUTE *oa = (OBJECT_ATTRIBUTE *)&mParam;
    mAnimating = ETrue;
    mSprite->mImageNumber = mState ? (mImage + 1) : mImage;
  }

  return ETrue;
}
