#include "BProcess.h"
#include "GMidBossFireballProcess.h"
#include "GPlayer.h"

const TInt EXPLODE_SPEED = 5;

static ANIMSCRIPT fireballExplodeAnimation[] = {
  ABITMAP(MID_BOSS_PROJECTILE_SLOT),
  ASTEP(EXPLODE_SPEED, IMG_FIREBALL_EXPLODE + 0),
  ASTEP(EXPLODE_SPEED, IMG_FIREBALL_EXPLODE + 1),
  ASTEP(EXPLODE_SPEED, IMG_FIREBALL_EXPLODE + 2),
  ASTEP(EXPLODE_SPEED, IMG_FIREBALL_EXPLODE + 3),
  AEND
};

class FireballSprite : public GAnchorSprite {
public:
  FireballSprite(GGameState *aGameState) : GAnchorSprite(aGameState, ENEMY_PRIORITY, MID_BOSS_PROJECTILE_SLOT, 0, STYPE_EBULLET) {
    mExploding = EFalse;
    mTimer = 128;
    //
  }

public:
  void Explode() {
    mExploding = ETrue;
    mTimer = 100000; // really big number
    vx = vy = 0;
    type = STYPE_DEFAULT;
    StartAnimation(fireballExplodeAnimation);
  }

  void Animate() OVERRIDE {
    if (mExploding) {
      BAnimSprite::Animate();
    }
    else {
      mTimer--;
      // instead of anim script, we just randomly choose an animation frame (for variety)
      mImageNumber = IMG_FIREBALL + Random(0, 4);
    }
  }

  TBool TimedOut() {
    return mTimer < 1;
  }

protected:
  TBool mExploding;
  TInt mTimer; // lifetime (before we explode)
};

const TFloat FRAMES_TO_HIT_PLAYER = 60;

GMidBossFireballProcess::GMidBossFireballProcess(GGameState *aGameState, TFloat aX, TFloat aY) {
  mGameState = aGameState;
  mSprite = new FireballSprite(mGameState);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->w = 16;
  mSprite->h = 16;
  // aim fireball at player
  const TFloat x1 = GPlayer::mSprite->x + (GPlayer::mSprite->vx * FRAMES_TO_HIT_PLAYER) + 16,
               x2 = mSprite->x + 12,
               y1 = GPlayer::mSprite->y + GPlayer::mSprite->vy * FRAMES_TO_HIT_PLAYER,
               y2 = mSprite->y - 24;

  mSprite->vx = (x1 - x2) / FRAMES_TO_HIT_PLAYER;
  mSprite->vy = (y1 - y2) / FRAMES_TO_HIT_PLAYER;
  mGameState->AddSprite(mSprite);
  mState = EFalse;
}

GMidBossFireballProcess::~GMidBossFireballProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GMidBossFireballProcess::RunBefore() {
  if (mState && mSprite->AnimDone()) {
    return EFalse;
  }
  return ETrue;
}

TBool GMidBossFireballProcess::RunAfter() {
  if (mState) {
    return ETrue;
  }
  if (mSprite->TimedOut() || mSprite->TestAndClearCType(STYPE_PLAYER | STYPE_ENEMY) || mSprite->Clipped()) {
    mSprite->Explode();
    mState = ETrue;
  }
  return ETrue;
}
