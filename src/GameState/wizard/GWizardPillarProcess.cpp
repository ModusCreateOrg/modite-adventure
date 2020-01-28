#include "GWizardPillarProcess.h"
#include "GPlayer.h"

const TInt16 PILLAR_SPEED = 2 * FACTOR;
const TUint16 EXPLODE_DELAY = 30 * FACTOR;

static ANIMSCRIPT warningAnimation[] = {
  ABITMAP(BOSS_PILLAR_SLOT),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  AFLIP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  ALOOP,
};

static ANIMSCRIPT explodeAnimation[] = {
  ABITMAP(BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ATYPE(STYPE_EBULLET),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  ASIZE(-8, 8, 24, 16),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 4),
  ATYPE(STYPE_DEFAULT),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 5),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 6),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 7),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 8),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 9),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 10),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 11),
  AEND,
};

static ANIMSCRIPT collapseAnimation[] = {
  ABITMAP(BOSS_PILLAR_SLOT),
  ATYPE(STYPE_DEFAULT),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 6),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 7),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 8),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 9),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 10),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 11),
  AEND,
};

static ANIMSCRIPT continuousAnimation[] = {
  ABITMAP(BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  ATYPE(STYPE_EBULLET),
  ASIZE(-8, 8, 24, 16),
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 5),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 3),
  ALOOP,
};

// constructor
GWizardPillarProcess::GWizardPillarProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aX, TFloat aY, TBool aFollowPlayer, TInt aStartDelay = 0)
    : GProcess(0, 0) {
  mGameState = aGameState;
  mParent = aParent;
  mSaveToStream = EFalse;
  mSprite = new GAnchorSprite(mGameState, BOSS_PILLAR_SLOT, 0);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->w = 16;
  mSprite->h = 8;
  mSprite->cy = 4;
  mSprite->cx = 0;
  mSprite->vy = 0;
//  mSprite->type = STYPE_EBULLET;
  mFollowPlayer = aFollowPlayer;
  mSprite->SetCMask(STYPE_PLAYER);
  mSprite->SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);
  mSprite->mHitStrength = 55;
  mGameState->AddSprite(mSprite);
  mSprite->StartAnimation(warningAnimation);
  mExploding = EFalse;
  mFrame = 0;
  mStartDelay = (aStartDelay > 0) ? (30 + aStartDelay) : 0;
}

GWizardPillarProcess::~GWizardPillarProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GWizardPillarProcess::RunBefore() {
  if (!mSprite->CanWalk(DIRECTION_UP, 0, 0) || !mSprite->CanWalk(DIRECTION_DOWN, 0, 0)) {
    printf("PILLAR OBSTRUCTED\n");

    return EFalse;
  }

  if (mFollowPlayer) {
    mSprite->x = GPlayer::mSprite->x + 16;
    mSprite->y = GPlayer::mSprite->y;
  }
  return ETrue;
}

TBool GWizardPillarProcess::RunAfter() {
  if (mSprite->TestAndClearCType(STYPE_PLAYER) && mExploding) {
    mSprite->StartAnimation(collapseAnimation);
  }

  if (mExploding) {
    if (!mParent->IsChanneling()) {
      mSprite->StartAnimation(collapseAnimation);
    }
    if (mSprite->AnimDone()) {
      return EFalse;
    }
  }

  if (mStartDelay-- == 0) {
    if (mParent->IsChanneling()) {
      mSprite->StartAnimation(continuousAnimation);
    }
    else {
      mSprite->StartAnimation(explodeAnimation);
    }
    mExploding = ETrue;
    mFollowPlayer = EFalse;
  }

  return ETrue;
}
