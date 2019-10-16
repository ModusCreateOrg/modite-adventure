#include "GFloorSwitchProcess.h"
#include "GGamePlayfield.h"

const TInt ANIM_SPEED = FRAMES_PER_SECOND;

#if 0
static ANIMSCRIPT switchOffAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_FLOOR_SWITCH),
  AEND,
};
static ANIMSCRIPT switchOnAnimation[]  = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_FLOOR_SWITCH + 1),
  AEND,
};
#endif

GFloorSwitchProcess::GFloorSwitchProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY, TBool aWooden) {
  mGameState = aGameState;
  mParam = aParam;
  mSprite = ENull;
  mImage = IMG_FLOOR_SWITCH + (aWooden ? 2 : 0);

  mSprite = new GAnchorSprite(mGameState, FLOOR_SWITCH_PRIORITY, ENVIRONMENT_SLOT, mImage, STYPE_OBJECT);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->w = mSprite->h = 32;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 3;
  mGameState->AddSprite(mSprite);
  mState = mAnimating = EFalse;
}

GFloorSwitchProcess::~GFloorSwitchProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GFloorSwitchProcess::RunBefore() {
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
  if (mSprite->cType & STYPE_PBULLET) {
    mSprite->cType = 0;
    mSprite->ClearFlags(SFLAG_CHECK);
    mState = !mState;
    printf("Toggle Floor Switch %s\n", mState ? "ON" : "OFF");
    mSprite->mImageNumber = mState ? (mImage + 1) : mImage;
  }
  return ETrue;
}
