#include "GCrateProcess.h"

static const TInt BREAK_SPEED = 5;

static ANIMSCRIPT crateAnimation[] = {
  ALABEL,
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(1000, IMG_CRATE + 0),
  ALOOP,
};

static ANIMSCRIPT hitAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ADELTA(1, 1),
  ASTEP(BREAK_SPEED, IMG_CRATE + 1),
  ADELTA(0, 0),
  ASTEP(BREAK_SPEED, IMG_CRATE + 2),
  ADELTA(-1, -1),
  ASTEP(BREAK_SPEED, IMG_CRATE + 1),
  ADELTA(0, 0),
  ASTEP(BREAK_SPEED, IMG_CRATE + 0),
  AEND,
};

static ANIMSCRIPT breakAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(BREAK_SPEED, IMG_CRATE + 0),
  ASTEP(BREAK_SPEED, IMG_CRATE + 1),
  ASTEP(BREAK_SPEED, IMG_CRATE + 2),
  ASTEP(BREAK_SPEED, IMG_CRATE + 3),
  ASTEP(BREAK_SPEED, IMG_CRATE + 4),
  AEND,
};

GCrateProcess::GCrateProcess(GGameState *aGameState, TInt aIp, TUint16 aParam, TFloat aX, TFloat aY)
    : GEnvironmentProcess(aGameState, aIp, aParam, aX, aY) {
  mAnimating = EFalse;
  mSprite = new GAnchorSprite(mGameState, CRATE_PRIORITY, ENVIRONMENT_SLOT, IMG_CRATE, STYPE_ENEMY);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->w = mSprite->h = 32;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 32;
  mSprite->mHitPoints = 3;
  mSprite->StartAnimation(crateAnimation);
  mGameState->AddSprite(mSprite);
}

GCrateProcess::~GCrateProcess() {
  //
}

TBool GCrateProcess::RunBefore() {
  return ETrue;
}

TBool GCrateProcess::RunAfter() {
  if (mAnimating) {
    if (mSprite->AnimDone()) {
      mAnimating = EFalse;
      mSprite->cType = 0;
      if (mSprite->mHitPoints <= 0) {
        GItemProcess::SpawnItem(mGameState, mIp, mParam, mSprite->x, mSprite->y);
        mGameState->EndProgram(mIp, ATTR_CRATE_GONE, mParam); // do not persist crate, but persist item
        printf("Crate broken param = %x %d\n", mParam, mParam);
        return EFalse;
      }
      mSprite->StartAnimation(crateAnimation);
    }
  }
  else if (mSprite->cType & STYPE_PBULLET) {
    mAnimating = ETrue;
    if (--mSprite->mHitPoints > 0) {
      mSprite->StartAnimation(hitAnimation);
    }
    else {
      mSprite->StartAnimation(breakAnimation);
    }
  }
  return ETrue;
}
