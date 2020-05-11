#include "GMidBossProjectileProcess.h"
#include "GPlayer.h"
#include "GProcess.h"

const TInt EXPLODE_SPEED = 5;

static ANIMSCRIPT fireballExplodeAnimation[] = {
    ABITMAP(BOSS_PROJECTILE_SLOT),
    ASTEP(EXPLODE_SPEED, IMG_FIREBALL_EXPLODE + 0),
    ASTEP(EXPLODE_SPEED, IMG_FIREBALL_EXPLODE + 1),
    ASTEP(EXPLODE_SPEED, IMG_FIREBALL_EXPLODE + 2),
    ASTEP(EXPLODE_SPEED, IMG_FIREBALL_EXPLODE + 3),
    AEND};

class ProjectileSprite : public GAnchorSprite {
public:
  ProjectileSprite(GGameState *aGameState)
      : GAnchorSprite(aGameState, PLAYER_PRIORITY - 1, BOSS_PROJECTILE_SLOT, 0,
                      STYPE_EBULLET) {
    Name("PROJECTILE");
    mExploding = EFalse;
    mTimer = 128;
    type = STYPE_EBULLET;
    SetCMask(
        STYPE_PLAYER | STYPE_PBULLET |
        STYPE_OBJECT); // collide with player, player attacks, and environment
    SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);
    mSpriteSheet =
        gResourceManager.LoadSpriteSheet(MID_BOSS_FIRE_PROJECTILE_BMP_SPRITES);
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
    } else {
      mTimer--;
      // instead of anim script, we just randomly choose an animation frame (for
      // variety)
      mImageNumber = IMG_FIREBALL + Random(0, 4);
    }
  }

  TBool TimedOut() { return mTimer < 1; }

protected:
  TBool mExploding;
  TInt mTimer; // lifetime (before we explode)
};

const TFloat FRAMES_TO_HIT_PLAYER = 60;

GMidBossProjectileProcess::GMidBossProjectileProcess(GGameState *aGameState,
                                                     TFloat aX, TFloat aY)
    : GProcess(ATTR_GONE) {
  mGameState = aGameState;
  mSprite = new ProjectileSprite(mGameState);
  mSprite->Name("IGNORE MID BOSS PROJECTILE");
  mSprite->type = STYPE_EBULLET;
  mSprite->mAttackStrength = 55;
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->w = 16;
  mSprite->h = 8;
  mSprite->cy = 4;
  // aim fireball at player
  const TFloat x1 = GPlayer::mSprite->x + 16 +
                    (GPlayer::mSprite->vx * FRAMES_TO_HIT_PLAYER) + 16,
               x2 = mSprite->x + 12,
               y1 = GPlayer::mSprite->y - 16 +
                    GPlayer::mSprite->vy * FRAMES_TO_HIT_PLAYER,
               y2 = mSprite->y - 24;

  mSprite->vx = (x1 - x2) / FRAMES_TO_HIT_PLAYER;
  mSprite->vy = (y1 - y2) / FRAMES_TO_HIT_PLAYER;
  mGameState->AddSprite(mSprite);
  mState = EFalse;
}

GMidBossProjectileProcess::~GMidBossProjectileProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GMidBossProjectileProcess::RunBefore() {
  if (mState && mSprite->AnimDone()) {
    return EFalse;
  }
  return ETrue;
}

TBool GMidBossProjectileProcess::RunAfter() {
  // when mState is true, it means the fireball explosion is animating
  if (!mState) {
    // these could all be one big if statement, but it's easier to edit this way
    // so we can reorder the tests.
    if (mSprite->TestAndClearCType(STYPE_PLAYER | STYPE_ENEMY)) {
      mSprite->Explode();
      mState = ETrue;
    } else if (mSprite->TestAndClearCType(STYPE_PBULLET)) {
      mSprite->type = STYPE_PBULLET;
      mSprite->cMask = STYPE_ENEMY | STYPE_OBJECT;
      mSprite->vx *= -1;
      mSprite->vy *= -1;
    }
    if (mSprite->TimedOut()) {
      mSprite->Explode();
      mState = ETrue;
    }
    if (mSprite->Clipped()) {
      mSprite->Explode();
      mState = ETrue;
    }
    if (!mSprite->CanWalk(mSprite->vx, mSprite->vy)) {
      mSprite->Explode();
      mState = ETrue;
    }
    mSprite->ClearCType(STYPE_ENEMY);
  }
  return ETrue;
}
