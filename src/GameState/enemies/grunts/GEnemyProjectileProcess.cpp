#include "GEnemyProjectileProcess.h"

#define DEBUGME
#undef DEBUGME

const TInt PROJECTILE_TIMEOUT = 80;
const TInt PROJECTILE_EXPIRE = 35;

GEnemyProjectileProcess::GEnemyProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aImg)
    : GProcess(0, 0) {
  mSaveToStream = EFalse;

  mSprite = new GAnchorSprite(aGameState, 0, aSlot, aImg);
  mSprite->type = STYPE_EBULLET;
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT); // collide with player, player attacks, and environment
  mSprite->SetFlags(SFLAG_CHECK);
  mTimer = PROJECTILE_TIMEOUT;
  mState = LIVE;

  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->mDirection = DIRECTION_UNSPECIFIED;

  aGameState->AddSprite(mSprite);
}

GEnemyProjectileProcess::~GEnemyProjectileProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GEnemyProjectileProcess::RunBefore() {
  return ETrue;
}

TBool GEnemyProjectileProcess::RunAfter() {
  if (mSprite->Clipped()) {
#ifdef DEBUGME
    printf("ENEMY PROJECTILE CLIPPED\n");
#endif
    return EFalse;
  }

  if (mState == DIE && mSprite->AnimDone()) {
    return EFalse;
  }

  if (mState == LIVE && !mSprite->CanWalk(mSprite->vx, mSprite->vy)) {
    Explode();
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    mTimer = PROJECTILE_EXPIRE;
    mState = DIE;
    return ETrue;
  }

  // Fixes the issue of hitting the arrow the same frame the flag gets removed
  if (mSprite->TestFlags(SFLAG_CHECK)) {
    if (mSprite->TestAndClearCType(STYPE_PLAYER | STYPE_ENEMY)) {
#ifdef DEBUGME
      printf("ENEMY PROJECTILE HIT TARGET\n");
#endif
      Explode();
      mSprite->ClearFlags(SFLAG_CHECK);
      mSprite->vx = mSprite->vy = 0;
      mTimer = PROJECTILE_EXPIRE;
      mState = DIE;
    } else if (mSprite->TestAndClearCType(STYPE_PBULLET)) {
#ifdef DEBUGME
      printf("PLAYER HIT PROJECTILE\n");
#endif
      mSprite->type = STYPE_PBULLET;
      mSprite->cMask = STYPE_ENEMY | STYPE_OBJECT;
      mSprite->ClearFlags(SFLAG_FLIP | SFLAG_FLOP | SFLAG_LEFT | SFLAG_RIGHT);
      Parry();
      mTimer = PROJECTILE_TIMEOUT;
    }
  }

  if (--mTimer < 1) {
#ifdef DEBUGME
    printf("ENEMY PROJECTILE TIMEOUT\n");
#endif
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    return EFalse;
  }

  return ETrue;
}
