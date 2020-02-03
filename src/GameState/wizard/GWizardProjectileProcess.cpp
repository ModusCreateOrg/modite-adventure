#include <math.h>
#include <GameState/player/GPlayer.h>
#include "GWizardProjectileProcess.h"

const TFloat PROJECTILE_VELOCITY = 3;
const TInt PROJECTILE_DELAY = 0.5 * FRAMES_PER_SECOND;

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
GWizardProjectileProcess::GWizardProjectileProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aX, TFloat aY, TFloat aAngle, TInt16 aAttribute)
    : GProcess(0, 0) {

  mAttribute = aAttribute;
  mParent = aParent;
  mTimer = 0;
  mAngle = aAngle;
  mSaveToStream = EFalse;
  mStep = 0;
  switch (mAttribute) {
    case ATTR_WIZARD_ENERGY:
      mSprite = new GAnchorSprite(aGameState, 0, ENERGY_WIZARD_PROJECTILE_SLOT);
      break;
    case ATTR_WIZARD_FIRE:
      mSprite = new GAnchorSprite(aGameState, 0, FIRE_WIZARD_PROJECTILE_SLOT);
      break;
    case ATTR_WIZARD_EARTH:
      mSprite = new GAnchorSprite(aGameState, 0, EARTH_WIZARD_PROJECTILE_SLOT);
      break;
    case ATTR_WIZARD_WATER:
      mSprite = new GAnchorSprite(aGameState, 0, WATER_WIZARD_PROJECTILE_SLOT);
      break;
    default:
      Panic("Projectile invalid mAttribute: %d\n", mAttribute);
  }
  mSprite->type = STYPE_EBULLET;
  mSprite->SetCMask(STYPE_PLAYER);
  mSprite->SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);
  mSprite->mHitStrength = mParent->mSprite->mHitStrength;

  mSprite->x = aX;
  mSprite->y = aY;
//   mSprite->x = aParent->mSprite->x + 16;
//   mSprite->y = aParent->mSprite->y;
  mSprite->w = 16;
  mSprite->h = 8;
  mSprite->cy = 4;

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
  if (mSprite->AnimDone()) {
    return EFalse;
  }

  if (mStep == 0 && (!mSprite->IsFloor(DIRECTION_DOWN, mSprite->vx, mSprite->vy) ||
      !mSprite->IsFloor(DIRECTION_UP, mSprite->vx, mSprite->vy))) {
    return EFalse;
  }

  return ETrue;
}

TBool GWizardProjectileProcess::RunAfter() {
  switch (mStep) {
    case 0:
      if (!mParent->IsChanneling()) {
        mStep++;
      }
      break;
    case 1:
      if (mTimer++ > PROJECTILE_DELAY) {
        mStep++;
      }
      break;
    case 2:
      mSprite->vx = mSprite->vy = PROJECTILE_VELOCITY;
      if (mAngle < 0) { // launch directly outward if angle is negative
        mSprite->vx *= SIN(mAngle);
        mSprite->vy *= COS(mAngle);
      } else {
        TFloat xx = mSprite->x - 16,
                yy = mSprite->y + 0;
        const TFloat angleToPlayer = atan2(GPlayer::mSprite->y - yy, GPlayer::mSprite->x - xx);
        // Angles are in radians
        mSprite->vx *= COS(angleToPlayer);
        mSprite->vy *= SIN(angleToPlayer);
      }
      mStep++;
    case 4:
      return ETrue;
    default:
      break;
  }

  if (!mSprite->IsFloor(DIRECTION_DOWN, mSprite->vx, mSprite->vy) ||
      !mSprite->IsFloor(DIRECTION_UP, mSprite->vx, mSprite->vy)) {
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    StartExplodeAnimation();
    mStep = 4;
  }

  if (mSprite->TestAndClearCType(STYPE_PLAYER) && mStep > 0) {
    printf("HIT PLAYER\n");
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    StartExplodeAnimation();
    mStep = 4;
  }

  return ETrue;
}
