#include "GLeverProcess.h"

const TInt ANIM_SPEED = FRAMES_PER_SECOND;

static ANIMSCRIPT leverLeftAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_LEVER),
  AEND,
};
static ANIMSCRIPT leverCenterAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_LEVER + 1),
  AEND,
};
static ANIMSCRIPT leverRightAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_LEVER + 2),
  AEND,
};

GLeverProcess::GLeverProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY) {
  mGameState = aGameState;
  mAnimating = EFalse;
  mDirection = ETrue;

  mSprite = new GAnchorSprite(mGameState, LEVER_PRIORITY, ENVIRONMENT_SLOT, IMG_LEVER, STYPE_OBJECT);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->w = mSprite->h = 32;
  mSprite->x = aX;
  mSprite->y = aY - 29;
  mSprite->ClearFlags(SFLAG_ANCHOR);
  mGameState->AddSprite(mSprite);
  mState = 1;
  mSprite->StartAnimation(leverCenterAnimation);
}

GLeverProcess::~GLeverProcess() {
  //
}

TBool GLeverProcess::RunBefore() {
  GGamePlayfield *p = mGameState->mGamePlayfield;

  TInt group = mAttribute->group,
       state = mAttribute->state;

  if (group) {
    if (mState == state) {
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

TBool GLeverProcess::RunAfter() {
  if (mAnimating) {
    if (mSprite->AnimDone()) {
      mAnimating = EFalse;
      mSprite->type = STYPE_OBJECT;
      mSprite->cType = 0;
      mSprite->SetFlags(SFLAG_CHECK);
      return ETrue;
    }
  }

  if (mSprite->cType & STYPE_PBULLET) {
    mSprite->cType = 0;
    mSprite->ClearFlags(SFLAG_CHECK);
    if (mDirection) {
      mState++;
      if (mState > 2) {
        mState = 1;
        mDirection = EFalse;
      }
    }
    else {
      mState--;
      if (mState < 0) {
        mState = 1;
        mDirection = ETrue;
      }
    }
    mAnimating = ETrue;
    switch (mState) {
      case 0:
        mSprite->StartAnimation(leverLeftAnimation);
        break;
      case 1:
        mSprite->StartAnimation(leverCenterAnimation);
        break;
      case 2:
        mSprite->StartAnimation(leverRightAnimation);
        break;
    }
  }

  return ETrue;
}
