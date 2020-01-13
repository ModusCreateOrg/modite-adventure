#include "GWizardProjectileProcess.h"

const TFloat PROJECTILE_VELOCITY = 3;
const TInt PROJECTILE_TIMEOUT = 90;

const TInt16 PROJECTILE_SPEED = 4;
const TInt16 EXPLODE_SPEED = 2;

static ANIMSCRIPT projectileAnimation[] = {
  ABITMAP(BOSS_PROJECTILE_SLOT),
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

static ANIMSCRIPT explodeAnimation[] = {
  ABITMAP(BOSS_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 5),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 6),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 7),
  ASTEP(EXPLODE_SPEED, IMG_WIZARD_PROJECTILE + 8),
  AEND,
};

// consructor
GWizardProjectileProcess::GWizardProjectileProcess(GGameState *aGameState, GWizardProcess *aParent, TInt aAngle)
    : GProcess(0, 0) {
  mSaveToStream = EFalse;
  mStep = 0;
  mTimer = PROJECTILE_TIMEOUT;
  mSprite = new GAnchorSprite(aGameState, 0, 0);
  mSprite->type = STYPE_EBULLET;
  mSprite->SetCMask(STYPE_PLAYER);
  mSprite->SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);
  mSprite->mHitStrength = 55;

  mSprite->x = aParent->mSprite->x + 16;
  mSprite->y = aParent->mSprite->y;
  mSprite->w = 16;
  mSprite->h = 8;
  mSprite->cy = 4;

  mSprite->vy = PROJECTILE_VELOCITY;
  switch (aAngle) {
    case 0:
      mSprite->vx = -PROJECTILE_VELOCITY;
      break;
    case 1:
      mSprite->vx = 0;
      break;
    default:
      mSprite->vx = PROJECTILE_VELOCITY;
      break;
  }
  aGameState->AddSprite(mSprite);
  mSprite->StartAnimation(projectileAnimation);
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
  if (mSprite->Clipped()) {
    return EFalse;
  }
  if (mTimer <= 0) {
    return EFalse;
  }
  if (mStep > 0 && mSprite->AnimDone()) {
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
    mSprite->StartAnimation(explodeAnimation);
    mStep++;
  }
  if (mSprite->vx < 0) {
    if (!mSprite->IsFloor(DIRECTION_LEFT, mSprite->vx, mSprite->vy)) {
      mSprite->vx = mSprite->vy = 0;
      mSprite->ClearFlags(SFLAG_CHECK);
      mSprite->StartAnimation(explodeAnimation);
      mStep++;
    }
  }
  if (mSprite->vx > 0) {
    if (!mSprite->IsFloor(DIRECTION_RIGHT, mSprite->vx, mSprite->vy)) {
      mSprite->vx = mSprite->vy = 0;
      mSprite->ClearFlags(SFLAG_CHECK);
      mSprite->StartAnimation(explodeAnimation);
      mStep++;
    }
  }
  if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
    printf("HIT PLAYER\n");
    mSprite->ClearFlags(SFLAG_CHECK);
    mSprite->vx = mSprite->vy = 0;
    mSprite->StartAnimation(explodeAnimation);
    mStep++;
    mTimer = 10000;
  }
 if (--mTimer < 1) {
    printf("TIMEOUT\n");
    mTimer = 10000;
    mSprite->ClearFlags(SFLAG_CHECK);
    mStep++;
    mSprite->StartAnimation(explodeAnimation);
  }

  return ETrue;
}
