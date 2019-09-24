#include "GCrateProcess.h"
#include "GItemProcess.h"
#include "GGamePlayfield.h"

static const TInt BREAK_SPEED = 5;

static ANIMSCRIPT breakAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(BREAK_SPEED, IMG_CRATE + 0),
  ASTEP(BREAK_SPEED, IMG_CRATE + 1),
  ASTEP(BREAK_SPEED, IMG_CRATE + 2),
  ASTEP(BREAK_SPEED, IMG_CRATE + 3),
  ASTEP(BREAK_SPEED, IMG_CRATE + 4),
  AEND,
};

GCrateProcess::GCrateProcess(GGameState *aGameState, TUint16 aParam, TFloat aX, TFloat aY) {
  mGameState = aGameState;
  mParam = aParam;
  mBreaking = EFalse;
  mSprite = new GAnchorSprite(999, ENVIRONMENT_SLOT, IMG_CRATE, STYPE_ENEMY);
  mSprite->cMask = STYPE_PBULLET;
  mSprite->cMask &= ~STYPE_PLAYER;
  mSprite->flags |= SFLAG_CHECK;
  mSprite->w = mSprite->h = 32;
  mSprite->cx = -16;
  mSprite->x = aX;
  mSprite->y = aY + 32;
  mGameState->AddSprite(mSprite);
}

GCrateProcess::~GCrateProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GCrateProcess::RunBefore() {
  return ETrue;
}

TBool GCrateProcess::RunAfter() {
  if (mBreaking) {
    if (mSprite->AnimDone()) {
      printf("Crate broken param = %x %d\n", mParam, mParam);
      GItemProcess::SpawnItem(mGameState, mParam, mSprite->x, mSprite->y);
      return EFalse;
    }
  }
  else if (mSprite->cType & STYPE_PBULLET) {
    mSprite->StartAnimation(breakAnimation);
    mBreaking = ETrue;
  }
  return ETrue;
}
