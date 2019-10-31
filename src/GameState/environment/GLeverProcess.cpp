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

  mSprite = new GAnchorSprite(mGameState, LEVER_PRIORITY, ENVIRONMENT_SLOT, IMG_LEVER, STYPE_ENEMY);
  mSprite->SetCMask(STYPE_PBULLET | STYPE_PLAYER);
  mSprite->w = 32;
  mSprite->h = 24;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 3;
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
  if (mSprite->TestAndClearCType(STYPE_PBULLET)) {
    if (!mAnimating) {
      mAnimating = ETrue;
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
  } else {
    mAnimating = EFalse;
  }

  return ETrue;
}
