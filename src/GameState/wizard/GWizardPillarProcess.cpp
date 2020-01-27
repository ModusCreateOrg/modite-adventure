#include "GWizardPillarProcess.h"
#include "GPlayer.h"

const TInt16 PILLAR_SPEED = 2 * FACTOR;
const TUint16 EXPLODE_DELAY = 30 * FACTOR;

static ANIMSCRIPT pillarAnimation[] = {
  ALABEL,
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  AFLIP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  AFLIP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  ALOOP,
};

static ANIMSCRIPT explodeAnimation[] = {
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ANULL(PILLAR_SPEED * 4),
  ASIZE(-8, 8, 24, 16),
  ATYPE(STYPE_EBULLET),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 0),
  ASTEP(PILLAR_SPEED, IMG_WIZARD_PILLAR + 1),
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

static ANIMSCRIPT pillarSpawn[] = {
  ABITMAP(BOSS_PILLAR_SLOT),
  ALABEL,
  ANULL(180),
  AEND,
};

// constructor
GWizardPillarProcess::GWizardPillarProcess(GGameState *aGameState, TFloat aX, TFloat aY, TBool aFollowPlayer, TInt aStartDelay = 0)
    : GProcess(0, 0) {
  mGameState = aGameState;
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
  mSprite->StartAnimation(pillarSpawn);
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
  if (mExploding && mSprite->AnimDone()) {
    return EFalse;
  }

  if (! mFollowPlayer && !mSprite->CanWalk(DIRECTION_UP, 0, -10)) {
    printf("REJECTED DIRECTION_UP\n");
    return EFalse;
  }

  if (! mFollowPlayer && !mSprite->CanWalk(DIRECTION_RIGHT, 32, 0)) {
    printf("REJECTED DIRECTION_RIGHT\n");

    return EFalse;
  }

  if (! mFollowPlayer && !mSprite->CanWalk(DIRECTION_LEFT, -32, 0)) {
    printf("REJECTED DIRECTION_LEFT\n");

    return EFalse;
  }


  if (! mFollowPlayer && !mSprite->CanWalk(DIRECTION_DOWN, 0, 10)) {
    printf("REJECTED DIRECTION_DOWN\n");

    return EFalse;
  }

  if (mStartDelay > 0) {
    mStartDelay--;


    if (mFollowPlayer) {

      mSprite->x = GPlayer::mSprite->x + 16;
      mSprite->y = GPlayer::mSprite->y;
    }
    return ETrue;
  }

  if (mFrame == 0 && ! mExploding) {
    mSprite->StartAnimation(pillarAnimation);

  }

  mFrame++;


  return ETrue;
}

TBool GWizardPillarProcess::RunAfter() {
  if (!mExploding) {
    if (mFrame > EXPLODE_DELAY) {
      mSprite->StartAnimation(explodeAnimation);
      return mExploding = ETrue;
    }

    if (mSprite->TestAndClearCType(STYPE_PLAYER)) {
      mSprite->StartAnimation(explodeAnimation);
      mExploding = ETrue;
    }
  }
  return ETrue;
}
