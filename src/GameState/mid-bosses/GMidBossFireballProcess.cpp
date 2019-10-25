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
    //
  }

public:
  void Animate() OVERRIDE {
    if (TestFlags(SFLAG_ANIMATE)) {
      BAnimSprite::Animate();
    }
    else {
      // instead of anim script, we just randomly choose an animation frame (for variety)
      mImageNumber = IMG_FIREBALL + Random(0, 4);
    }
  }
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
  if (mSprite->TestAndClearCType(STYPE_PLAYER) || mSprite->Clipped()) {
    mSprite->vx = mSprite->vy = 0;
    mSprite->type = STYPE_DEFAULT;
    mSprite->StartAnimation(fireballExplodeAnimation);
    mState = ETrue;
  }
  return ETrue;
}
