#include "GLeverProcess.h"
#include "GGamePlayfield.h"

const TInt ANIM_SPEED = FRAMES_PER_SECOND;

static ANIMSCRIPT leverLeftAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_LEVER),
  AEND,
};
static ANIMSCRIPT leverCenterAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_LEVER+1),
  AEND,
};
static ANIMSCRIPT leverRightAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(ANIM_SPEED, IMG_LEVER+2),
  AEND,
};

GLeverProcess::GLeverProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY) {
  mGameState = aGameState;
  mParam     = aParam;
  mState     = 0;
  mAnimating = EFalse;
  mDirection = ETrue;
  mSprite    = ENull;

  mSprite = new GAnchorSprite(mGameState, LEVER_PRIORITY, ENVIRONMENT_SLOT, IMG_LEVER, STYPE_OBJECT);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->w     = mSprite->h = 32;
  mSprite->cx    = -16;
  mSprite->x     = aX;
  mSprite->y     = aY + 3;
  mGameState->AddSprite(mSprite);
}

GLeverProcess::~GLeverProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GLeverProcess::RunBefore() {
  return ETrue;
}

TBool GLeverProcess::RunAfter() {
 if (mAnimating) {
    if(mSprite->AnimDone()) {
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
    printf("\n\nmState: %d %s\n", mState, mDirection ? "TRUE" : "FALSE");
    if (mDirection) {
      mState++;
      if (mState > 2) {
        mState = 1;
        mDirection = EFalse;
      }
    } else {
      mState--;
      if (mState < 0) {
        mState = 1;
        mDirection = ETrue;
      }
    }
    printf("mState: %d %s\n", mState, mDirection ? "TRUE" : "FALSE");
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
