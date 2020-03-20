#include <math.h>
#include "GEnemyCrystalProcess.h"

#define DEBUGME
#undef DEBUGME

const TFloat PROJECTILE_VELOCITY = 3;
const TInt PROJECTILE_TIMEOUT = 80;
const TInt PROJECTILE_EXPIRE = 35;
const TInt PROJECTILE_DAMAGE = 55;
const TInt16 PROJECTILE_SPEED = 4;

static ANIMSCRIPT shootAnimation[] = {
  ABITMAP(BOSS_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, 0),
  ASTEP(PROJECTILE_SPEED, 1),
  ASTEP(PROJECTILE_SPEED, 2),
  ASTEP(PROJECTILE_SPEED, 3),
  ALOOP,
};

static ANIMSCRIPT explodeAnimation[] = {
  ABITMAP(BOSS_PROJECTILE_SLOT),
  ASTEP(PROJECTILE_SPEED, 4),
  ASTEP(PROJECTILE_SPEED, 5),
  ASTEP(PROJECTILE_SPEED, 6),
  ASTEP(PROJECTILE_SPEED, 7),
  AEND,
};

GEnemyCrystalProcess::GEnemyCrystalProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TUint16 aSlot, TUint16 aImg)
    : GProcess(0, 0) {
  mSaveToStream = EFalse;

  mSprite = new GAnchorSprite(aGameState, 0, aSlot, aImg);
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(PROJECTILE_CRYSTAL_BMP_SPRITES);
  mSprite->type = STYPE_EBULLET;
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT); // collide with player, player attacks, and environment
  mSprite->SetFlags(SFLAG_CHECK);
  mSprite->mAttackStrength = PROJECTILE_DAMAGE;
  mTimer = PROJECTILE_TIMEOUT;
  mState = LIVE;

  mSprite->x = aX;
  mSprite->y = aY;
  mAngle = aAngle;
  mSprite->w = 16;
  mSprite->h = 16;
  mSprite->cx = 0;
  mSprite->cy -= 8;

  // Angles are in radians
  mSprite->vx = cos(mAngle) * PROJECTILE_VELOCITY;
  mSprite->vy = sin(mAngle) * PROJECTILE_VELOCITY;

  aGameState->AddSprite(mSprite);
  mSprite->StartAnimation(shootAnimation);
#ifdef DEBUGME
  printf("ENEMY PROJECTILE at %f,%f with velocity of %f %f\n", mSprite->x, mSprite->y, mSprite->vx, mSprite->vy);
#endif
}

GEnemyCrystalProcess::~GEnemyCrystalProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GEnemyCrystalProcess::RunBefore() {
  return ETrue;
}

TBool GEnemyCrystalProcess::RunAfter() {
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
    mSprite->StartAnimation(explodeAnimation);
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
      mSprite->StartAnimation(explodeAnimation);
      mSprite->ClearFlags(SFLAG_CHECK);
      mSprite->vx = mSprite->vy = 0;
      mTimer = PROJECTILE_EXPIRE;
      mState = DIE;
    } else if (mSprite->TestAndClearCType(STYPE_PBULLET)) {
#ifdef DEBUGME
      printf("PLAYER HIT PROJECTILE\n");
#endif
      // TODO: @jaygarcia -- SFX here
      mSprite->type = STYPE_PBULLET;
      mSprite->cMask = STYPE_ENEMY | STYPE_OBJECT;
      mSprite->ClearFlags(SFLAG_FLIP | SFLAG_FLOP | SFLAG_LEFT | SFLAG_RIGHT);
      mSprite->vx *= -1;
      mSprite->vy *= -1;
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
