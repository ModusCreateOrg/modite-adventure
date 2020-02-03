#include "GWizardPillarProcess.h"
#include "GPlayer.h"

const TInt16 PILLAR_SPEED = 2 * FACTOR;

static ANIMSCRIPT warningAnimation[] = {
  ABITMAP(BOSS_PILLAR_SLOT),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ADELTA(2, -4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ADELTA(0, -4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ADELTA(-2, -4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ADELTA(0, -4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  AEND,
};

static ANIMSCRIPT explodeAnimation[] = {
  ABITMAP(BOSS_PILLAR_SLOT),
  ATYPE(STYPE_EBULLET),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 2),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 3),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 5),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 4),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 3),
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

// constructor
GWizardPillarProcess::GWizardPillarProcess(GGameState *aGameState, GWizardProcess *aParent, TFloat aAngle, TFloat aDistance, TBool aFollowPlayer, TUint16 aMaxDuration)
    : GProcess(0, 0) {
  mGameState = aGameState;
  mParent = aParent;
  mMaxDuration = aMaxDuration;
  mSaveToStream = EFalse;
  mSprite = new GAnchorSprite(mGameState, 0, BOSS_PILLAR_SLOT);
  mSprite->w = 16;
  mSprite->h = 8;
  mSprite->mDy = -4;
  mSprite->cy = 0;
  mSprite->cx = 0;
//  mSprite->type = STYPE_EBULLET;
  mFollowPlayer = aFollowPlayer;
  if (mFollowPlayer) {
    mSprite->x = GPlayer::mSprite->mLastX + 16;
    mSprite->y = GPlayer::mSprite->mLastY + 1; // prevent starting hidden behind player
  } else {
    mSprite->x = mParent->mSprite->x + COS(aAngle) * aDistance * 1.5 + 16;
    mSprite->y = mParent->mSprite->y + SIN(aAngle) * aDistance;
  }
  mSprite->SetCMask(STYPE_PLAYER);
  mSprite->ClearFlags(SFLAG_CHECK);
  mSprite->SetFlags(SFLAG_RENDER_SHADOW | SFLAG_KNOCKBACK);
  mSprite->mHitStrength = mParent->mSprite->mHitStrength;
  mGameState->AddSprite(mSprite);
  mSprite->StartAnimation(warningAnimation);
  mExploding = EFalse;
  mFrame = 0;
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
    mSprite->StartAnimation(collapseAnimation);
    mFollowPlayer = EFalse;
    mExploding = ETrue;
    if (mFrame == 0) {
      printf("PILLAR OBSTRUCTED\n");
      return EFalse;
    }
  }

  if (mFollowPlayer) {
    mSprite->x = ((mSprite->x * 20 * FACTOR) + GPlayer::mSprite->mLastX + 16) / (20 * FACTOR + 1);
    mSprite->y = ((mSprite->y * 20 * FACTOR) + GPlayer::mSprite->mLastY) / (20 * FACTOR + 1);
  }

  return ETrue;
}

TBool GWizardPillarProcess::RunAfter() {
  mFrame++;
  if (!mExploding && mSprite->TestAndClearCType(STYPE_PLAYER)) {
    mSprite->StartAnimation(collapseAnimation);
  }

  if (mSprite->AnimDone()) {
    if (mExploding) {
      return EFalse;
    } else {
      mSprite->SetFlags(SFLAG_CHECK);
      if (mParent->IsChanneling() && mFrame < mMaxDuration) {
        mSprite->StartAnimation(explodeAnimation);
        mFollowPlayer = EFalse;
      } else {
        mSprite->StartAnimation(collapseAnimation);
        mExploding = ETrue;
      }
    }
  }

  return ETrue;
}
