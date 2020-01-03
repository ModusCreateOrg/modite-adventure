#include "GFloorSwitchProcess.h"

const TInt ANIM_SPEED = FRAMES_PER_SECOND;

GFloorSwitchProcess::GFloorSwitchProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY, TBool aWooden)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY) {
  mSprite = ENull;
  mImage = IMG_FLOOR_SWITCH + (aWooden ? 2 : 0);

  mAttribute = aWooden ? ATTR_FLOOR_SWITCH_WOOD : ATTR_FLOOR_SWITCH;
  mSprite = new GAnchorSprite(mGameState, FLOOR_SWITCH_PRIORITY, ENVIRONMENT_SLOT, mImage, STYPE_OBJECT);
  mSprite->Name("ENVIRONMENT FLOOR SWITCH");
  mSprite->SetFlags(SFLAG_BELOW);// render below other sprites
  mSprite->SetCMask(STYPE_PBULLET);
  mSprite->ClearCMask(STYPE_PLAYER);
  mSprite->w = mSprite->h = 32;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 3;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(GLOBAL_OBJECT_LAYER_BMP_SPRITES);
  mGameState->AddSprite(mSprite);
  mState = mAnimating = EFalse;
}

GFloorSwitchProcess::~GFloorSwitchProcess() {
  //
}

TBool GFloorSwitchProcess::RunBefore() {
  GGamePlayfield *p = mGameState->mGamePlayfield;

  TInt group = mObjectAttribute->attr.group;
  if (group) {
    if (mState) {
      if (mObjectAttribute->attr.order != OA_ORDER_ANY) {
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
      return ETrue;
    }
  }

  if (mSprite->TestAndClearCType(STYPE_PBULLET)) {
    if (!mAnimating) {
      mState = !mState;
      OBJECT_ATTRIBUTE *oa = (OBJECT_ATTRIBUTE *)&mParam;
      mAnimating = ETrue;
      mSprite->mImageNumber = mState ? (mImage + 1) : mImage;
    }
  } else {
    mAnimating = EFalse;
  }

  return ETrue;
}
