#include <math.h>
#include "GWizardProjectileProcess.h"

const TFloat PROJECTILE_VELOCITY = 3;
const TInt PROJECTILE_TIMEOUT = 90;

const TInt16 PROJECTILE_SPEED = 4;
const TInt16 EXPLODE_SPEED = 2;

static ANIMSCRIPT projectileFireAnimation[] = {
  ABITMAP(FIRE_WIZARD_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 0),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 1),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 4),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 1),
  ALOOP,
};
static ANIMSCRIPT projectileWaterAnimation[] = {
  ABITMAP(WATER_WIZARD_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 0),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 1),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 4),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 1),
  ALOOP,
};

static ANIMSCRIPT projectileEnergyAnimation[] = {
  ABITMAP(ENERGY_WIZARD_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 0),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 1),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 4),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 1),
  ALOOP,
};

static ANIMSCRIPT projectileEarthAnimation[] = {
  ABITMAP(EARTH_WIZARD_PROJECTILE_SLOT),
  ALABEL,
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 0),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 1),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 4),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 3),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 2),
  ASTEP(PROJECTILE_SPEED, IMG_WIZARD_PROJECTILE + 1),
  ALOOP,
};

static ANIMSCRIPT explodeEnergyAnimation[] = {
  ABITMAP(ENERGY_WIZARD_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 8),
  AEND,
};

static ANIMSCRIPT explodeFireAnimation[] = {
  ABITMAP(FIRE_WIZARD_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 8),
  AEND,
};

static ANIMSCRIPT explodeEarthAnimation[] = {
  ABITMAP(EARTH_WIZARD_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 8),
  AEND,
};

static ANIMSCRIPT explodeWaterAnimation[] = {
  ABITMAP(WATER_WIZARD_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 8),
  AEND,
};

void GWizardProjectileProcess::StartProjectileAnimation() {
  switch (mAttribute) {
    case ATTR_WIZARD_ENERGY:
      mSprite->StartAnimation(projectileEnergyAnimation);
      break;
    case ATTR_WIZARD_FIRE:
      mSprite->StartAnimation(projectileFireAnimation);
      break;
    case ATTR_WIZARD_EARTH:
      mSprite->StartAnimation(projectileEarthAnimation);
      break;
    case ATTR_WIZARD_WATER:
      mSprite->StartAnimation(projectileWaterAnimation);
      break;
  }
}

void GWizardProjectileProcess::StartExplodeAnimation() {
  mExploding = ETrue;
  switch (mAttribute) {
    case ATTR_WIZARD_ENERGY:
      mSprite->StartAnimation(explodeEnergyAnimation);
      break;
    case ATTR_WIZARD_FIRE:
      mSprite->StartAnimation(explodeFireAnimation);
      break;
    case ATTR_WIZARD_EARTH:
      mSprite->StartAnimation(explodeEarthAnimation);
      break;
    case ATTR_WIZARD_WATER:
      mSprite->StartAnimation(explodeWaterAnimation);
      break;
    default:
      Panic("StartExplision: invalid mAttribute: %d\n", mAttribute);
  }
}
// consructor
GWizardProjectileProcess::GWizardProjectileProcess(GGameState *aGameState, TFloat aX, TFloat aY, TFloat aAngle, TInt16 aAttribute)
    : GProcess(0, 0) {

  mAttribute = aAttribute;
  mSaveToStream = EFalse;
  mStep = 0;
  mTimer = PROJECTILE_TIMEOUT;
  mSprite = new GAnchorSprite(aGameState, 0, 0);
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
  printf("WIZARD PROJECTILE at %f,%f\n", mSprite->x, mSprite->y);
}

GWizardProjectileProcess::~GWizardProjectileProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GWizardProjectileProcess::RunBefore() {
  if (mExploding && mSprite->AnimDone()) {
//Todo: @jaygarcia
// =======
//   if (mSprite->Clipped()) {
//     return EFalse;
//   }
//   if (mTimer <= 0) {
//     return EFalse;
//   }
//   if (mStep > 0 && mSprite->AnimDone()) {
    return EFalse;
  }

  return ETrue;
}

TBool GWizardProjectileProcess::RunAfter() {
  if (mSprite->Clipped()) {
    return EFalse;
  }
  if (!mSprite->IsFloor(DIRECTION_DOWN, mSprite->vx, mSprite->vy)) {
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    StartExplodeAnimation();
    mStep++;
  }
  if (mSprite->vx < 0) {
    if (!mSprite->IsFloor(DIRECTION_LEFT, mSprite->vx, mSprite->vy)) {
      mSprite->vx = mSprite->vy = 0;
      mSprite->ClearFlags(SFLAG_CHECK);
      StartExplodeAnimation();
      mStep++;
    }
  }
  if (mSprite->vx > 0) {
    if (!mSprite->IsFloor(DIRECTION_RIGHT, mSprite->vx, mSprite->vy)) {
      mSprite->vx = mSprite->vy = 0;
      mSprite->ClearFlags(SFLAG_CHECK);
      StartExplodeAnimation();
      mStep++;
    }
  }
  if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
    printf("HIT PLAYER\n");
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    StartExplodeAnimation();
    mStep++;
    mTimer = 10000;
  }
 if (--mTimer < 1) {
    printf("TIMEOUT\n");
    mTimer = 10000;
    mSprite->ClearFlags(SFLAG_CHECK);
    mStep++;
    StartExplodeAnimation();
  }

  return ETrue;
}
