#include "GFloorSwitchProcess.h"
#include "GGamePlayfield.h"

const TInt ANIM_SPEED = FRAMES_PER_SECOND;

static ANIMSCRIPT switchOffAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_FLOOR_SWITCH),
  AEND,
};
static ANIMSCRIPT switchOnAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_FLOOR_SWITCH+1),
  AEND,
};
GFloorSwitchProcess::GFloorSwitchProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY) {
  mGameState = aGameState;
  mParam     = aParam;
  mSprite    = ENull;

  mSprite = new GAnchorSprite(999, ENVIRONMENT_SLOT, IMG_FLOOR_SWITCH, STYPE_OBJECT);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->w     = mSprite->h = 32;
  mSprite->cx    = -16;
  mSprite->x     = aX;
  mSprite->y     = aY + 3;
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
    if(mSprite->AnimDone()) {
      mSprite->cType = 0;
      mSprite->flags |= SFLAG_CHECK;
      return ETrue;
    }
  }
  if (mSprite->cType & STYPE_PBULLET) {
    mSprite->cType = 0;
    mSprite->flags &= ~SFLAG_CHECK;
    mState = !mState;
    printf("Toggle Floor Switch %s\n", mState ? "ON" : "OFF");
    mSprite->mImageNumber = mState ? (IMG_FLOOR_SWITCH + 1) : IMG_FLOOR_SWITCH;
  }
  return ETrue;
}
