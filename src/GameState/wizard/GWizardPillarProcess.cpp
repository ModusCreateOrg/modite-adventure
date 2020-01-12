#include "GWizardPillarProcess.h"

const TInt16 PILLAR_SPEED = 3;
const TFloat PILLAR_VELOCITY = 1.5;

static ANIMSCRIPT pillarAnimation[] = {
  ABITMAP(BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 3),
  ALOOP,
};

static ANIMSCRIPT explodeAnimation[] = {
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 5),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 6),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 7),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 8),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 9),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 10),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 11),
  AEND,
};

// constructor
GWizardPillarProcess::GWizardPillarProcess(GGameState *aGameState, TFloat aX, TFloat aY)
    : GProcess(0, 0) {
  mGameState = aGameState;
  mSaveToStream = EFalse;
  mSprite = new GAnchorSprite(mGameState, BOSS_PILLAR_SLOT, 0);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->w = 16;
  mSprite->h = 8;
  mSprite->cy = 4;
  mSprite->vy = -PILLAR_VELOCITY;
  mSprite->type = STYPE_EBULLET;
  mSprite->SetCMask(STYPE_PLAYER);
  mSprite->SetFlags(SFLAG_CHECK);
  mSprite->mHitStrength = 55;
  mGameState->AddSprite(mSprite);
  mSprite->StartAnimation(pillarAnimation);
  mExploding = EFalse;
}

GWizardPillarProcess::~GWizardPillarProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GWizardPillarProcess::RunBefore() {
  if (mSprite->Clipped() || (mExploding && mSprite->AnimDone())) {
    return EFalse;
  }
  return ETrue;
}

TBool GWizardPillarProcess::RunAfter() {
  if (!mExploding) {
    if (!mSprite->CanWalk(DIRECTION_UP, mSprite->vx, mSprite->vy)) {
      mSprite->StartAnimation(explodeAnimation);
      mExploding = ETrue;
    }
    if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
      mSprite->StartAnimation(explodeAnimation);
      mExploding = ETrue;
    }
  }
  return ETrue;
}
