#include "GSpikesProcess.h"
#include "GGamePlayfield.h"

const TUint16 SPIKE_SPEED = 3;

static const ANIMSCRIPT spikesAnimation[] = {
  ABITMAP(ENVIRONMENT_SLOT),
  ASTEP(SPIKE_SPEED, IMG_SPIKES),
  ASTEP(SPIKE_SPEED, IMG_SPIKES + 1),
  ATYPE(STYPE_EBULLET),
  ASTEP(SPIKE_SPEED, IMG_SPIKES + 2),
  ASTEP(SPIKE_SPEED, IMG_SPIKES + 3),
  ATYPE(STYPE_DEFAULT),
  ASTEP(SPIKE_SPEED, IMG_SPIKES + 2),
  ASTEP(SPIKE_SPEED, IMG_SPIKES + 1),
  ASTEP(SPIKE_SPEED, IMG_SPIKES),
  AEND,
};

GSpikesProcess::GSpikesProcess(GGameState *aGameState, TFloat aX, TFloat aY) {
  mGameState = aGameState;
  mSprite    = new GAnchorSprite(999, ENVIRONMENT_SLOT, IMG_SPIKES);
  mSprite->cMask = STYPE_DEFAULT;
  mSprite->w = mSprite->h = 32;
  mSprite->x     = aX;
  mSprite->y     = aY;
  mGameState->AddSprite(mSprite);
  mState = EFalse;
  mTimer = Random(FRAMES_PER_SECOND, 4 * FRAMES_PER_SECOND);
}

GSpikesProcess::~GSpikesProcess() {
  //
  if (mSprite) {
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GSpikesProcess::RunBefore() {
  if (mState) {
    if (mSprite->AnimDone()) {
      mTimer = Random(FRAMES_PER_SECOND, 4 * FRAMES_PER_SECOND);
      mState = EFalse;
    }
  } else {
    if (--mTimer < 0) {
      mState = ETrue;
      mSprite->StartAnimation(spikesAnimation);
    }

  }
  return ETrue;
}

TBool GSpikesProcess::RunAfter() {
  return ETrue;
}
