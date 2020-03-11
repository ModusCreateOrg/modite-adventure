#include <math.h>
#include "GFinalBossProjectileProcess.h"

#define DEBUGME
#undef DEBUGME

const TFloat PROJECTILE_VELOCITY = 3;
const TInt PROJECTILE_TIMEOUT = 90;

const TInt16 PROJECTILE_SPEED = 4;
const TInt16 EXPLODE_SPEED = 2;

static ANIMSCRIPT projectileFireAnimation[] = {
  ABITMAP(FIRE_FINAL_BOSS_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 1),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 4),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 1),
  ALOOP,
};
static ANIMSCRIPT projectileWaterAnimation[] = {
  ABITMAP(WATER_FINAL_BOSS_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 1),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 4),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 1),
  ALOOP,
};

static ANIMSCRIPT projectileEnergyAnimation[] = {
  ABITMAP(ENERGY_FINAL_BOSS_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 1),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 4),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 1),
  ALOOP,
};

static ANIMSCRIPT projectileEarthAnimation[] = {
  ABITMAP(EARTH_FINAL_BOSS_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 0),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 1),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 4),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 1),
  ALOOP,
};

static ANIMSCRIPT explodeEnergyAnimation[] = {
  ABITMAP(ENERGY_FINAL_BOSS_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 8),
  AEND,
};

static ANIMSCRIPT explodeFireAnimation[] = {
  ABITMAP(FIRE_FINAL_BOSS_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 8),
  AEND,
};

static ANIMSCRIPT explodeEarthAnimation[] = {
  ABITMAP(EARTH_FINAL_BOSS_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 8),
  AEND,
};

static ANIMSCRIPT explodeWaterAnimation[] = {
  ABITMAP(WATER_FINAL_BOSS_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_FINAL_BOSS_PROJECTILE + 8),
  AEND,
};

void GFinalBossProjectileProcess::StartProjectileAnimation() {
  switch (mAttribute) {
    case ATTR_FINAL_BOSS_ENERGY:
      mSprite->StartAnimation(projectileEnergyAnimation);
      break;
    case ATTR_FINAL_BOSS_FIRE:
      mSprite->StartAnimation(projectileFireAnimation);
      break;
    case ATTR_FINAL_BOSS_EARTH:
      mSprite->StartAnimation(projectileEarthAnimation);
      break;
    case ATTR_FINAL_BOSS_WATER:
      mSprite->StartAnimation(projectileWaterAnimation);
      break;
  }
}

void GFinalBossProjectileProcess::StartExplodeAnimation() {
#ifdef DEBUGME
  printf("%s startExplode\n", mSprite->mName);
#endif
  mExploding = ETrue;
  mSprite->vx = mSprite->vy = 0;
  mSprite->ClearFlags(SFLAG_CHECK | SFLAG_MOVE);
  mStep++;
  mTimer = 10000;
  switch (mAttribute) {
    case ATTR_FINAL_BOSS_ENERGY:
      mSprite->StartAnimation(explodeEnergyAnimation);
      break;
    case ATTR_FINAL_BOSS_FIRE:
      mSprite->StartAnimation(explodeFireAnimation);
      break;
    case ATTR_FINAL_BOSS_EARTH:
      mSprite->StartAnimation(explodeEarthAnimation);
      break;
    case ATTR_FINAL_BOSS_WATER:
      mSprite->StartAnimation(explodeWaterAnimation);
      break;
    default:
      Panic("StartExplision: invalid mAttribute: %d\n", mAttribute);
  }
}
// consructor
GFinalBossProjectileProcess::GFinalBossProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TInt16 aAttribute)
    : GProcess(0, 0) {

  mAttribute = aAttribute;
  mSaveToStream = EFalse;
  mStep = 0;
  mTimer = PROJECTILE_TIMEOUT;
  mSprite = new GAnchorSprite(aGameState, 0, 0);
  {
    char buf[128];
    sprintf(buf, "Final Boss Projectile %p", this);
    mSprite->Name(buf);
  }
  mSprite->type = STYPE_EBULLET;
  mSprite->SetCMask(STYPE_PLAYER);
  mSprite->SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);
  mSprite->mAttackStrength = 55;

  mSprite->x = aX;
  mSprite->y = aY;
  //   mSprite->x = aParent->mSprite->x + 16;
  //   mSprite->y = aParent->mSprite->y;
  mSprite->w = 16;
  mSprite->h = 8;
  mSprite->cy = 4;

  // Angles are in radians
  mSprite->vx = cos(aAngle) * PROJECTILE_VELOCITY;
  mSprite->vy = sin(aAngle) * PROJECTILE_VELOCITY;
  aGameState->AddSprite(mSprite);
  StartProjectileAnimation();
#ifdef DEBUGME
  printf("FINAL_BOSS PROJECTILE %p at %f,%f\n", this, mSprite->x, mSprite->y);
#endif
}

GFinalBossProjectileProcess::~GFinalBossProjectileProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GFinalBossProjectileProcess::RunBefore() {
  if (mExploding && mSprite->AnimDone()) {
#ifdef DEBUGME
    printf("Final Boss Projectile %p explosion finished\n", this);
#endif
    // projectile has exploded, we're done
    return EFalse;
  }

  return ETrue;
}

TBool GFinalBossProjectileProcess::RunAfter() {
  if (mSprite->Clipped()) {
    return EFalse;
  }
  if (mExploding) {
    return ETrue;
  }
  if (!mSprite->CanWalk(mSprite->vx, mSprite->vy)) {
#ifdef DEBUGME
    printf("Final Boss Projectile %p Hit Wall UP\n", this);
#endif
    StartExplodeAnimation();
    return ETrue;
  }
  if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
#ifdef DEBUGME
    printf("Final Boss Projectile %p HIT PLAYER\n", this);
#endif
    StartExplodeAnimation();
    mTimer = 10000;
    return ETrue;
  }
  if (--mTimer < 1) {
#ifdef DEBUGME
    printf("Final Boss Projectile %p TIMEOUT\n", this);
#endif
    StartExplodeAnimation();
  }

  return ETrue;
}
