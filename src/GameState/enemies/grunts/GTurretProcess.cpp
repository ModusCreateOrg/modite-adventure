#include "GTurretProcess.h"
#include "GEnemyCrystalProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt IDLE_SPEED = 6 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = 4 / FACTOR;
const TFloat ATTACK_DISTANCE = 6 * TILESIZE;
const TUint8 BLINK_TIME = 4 * FACTOR;
const TUint8 SHIMMER_TIMER = 4 * FACTOR;

// region  ANIMATIONS {{{

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

/**
 * Animations
 */

/*
 ___    _ _         ______       _           _           _
|_ _|__| | | ___   / / ___|  ___| | ___  ___| |_ ___  __| |
 | |/ _` | |/ _ \ / /\___ \ / _ \ |/ _ \/ __| __/ _ \/ _` |
 | | (_| | |  __// /  ___) |  __/ |  __/ (__| ||  __/ (_| |
|___\__,_|_|\___/_/  |____/ \___|_|\___|\___|\__\___|\__,_|
 */

ANIMSCRIPT idleAnimation[] = {
  ABITMAP(TURRET_SLOT),
  ALABEL,
  ADELTA(0, 0),
  ASIZE(10, 14, 22, 16),
  ASTEP(IDLE_SPEED, 0),

  ADELTA(0, -1),
  ASIZE(11, 14, 21, 16),
  // ASIZE(9, 14, 23, 16),
  ASTEP(IDLE_SPEED, 0),

  ADELTA(0, -2),
  ASIZE(12, 14, 20, 16),
  // ASIZE(8, 14, 24, 16),
  ASTEP(IDLE_SPEED * 2, 0),

  ADELTA(0, -1),
  ASIZE(11, 14, 21, 16),
  // ASIZE(9, 14, 23, 16),
  ASTEP(IDLE_SPEED, 0),

  ADELTA(0, 0),
  ASIZE(10, 14, 22, 16),
  ASTEP(IDLE_SPEED, 0),

  ADELTA(0, 1),
  ASIZE(9, 14, 23, 16),
  // ASIZE(11, 14, 21, 16),
  ASTEP(IDLE_SPEED, 0),

  ADELTA(0, 2),
  ASIZE(8, 14, 24, 16),
  // ASIZE(12, 14, 20, 16),
  ASTEP(IDLE_SPEED * 2, 0),

  ADELTA(0, 1),
  ASIZE(9, 14, 23, 16),
  // ASIZE(11, 14, 21, 16),
  ASTEP(IDLE_SPEED, 0),
  ALOOP
};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GTurretProcess::GTurretProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParam)
    : GGruntProcess(aGameState, aIp, aX, aY, TURRET_SLOT, aParam, VELOCITY, ATTR_TURRET) {
  mSprite->Name("ENEMY TURRET");
  mSprite->w = 22;
  mSprite->h = 16;
  mSprite->cx = 10;
  mSprite->cy = 16;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CRYSTAL_BMP_SPRITES);

  mHitState = 0;
  mTaunt = EFalse;
  mSpriteImageNumber = 0;
  mShimmerTimer = SHIMMER_TIMER;

  SetStatMultipliers(5.0, 10.0, 5.0);
  mSprite->StartAnimation(idleAnimation);
}

GTurretProcess::~GTurretProcess() = default;

void GTurretProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  const TInt lastDx = mSprite->mDx;
  const TInt lastDy = mSprite->mDy;
  GGruntProcess::NewState(aState, aDirection);
  mSprite->mDx = lastDx;
  mSprite->mDy = lastDy;
}

void GTurretProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  if (mSprite->AnimDone()) {
    mSprite->StartAnimation(idleAnimation);
  }
}

void GTurretProcess::Attack(DIRECTION aDirection) {
  // fire 1-3 projectiles
  TPoint p = mSprite->Center();
  p.Offset(0, -24);
  TPoint playerCenter = GPlayer::mSprite->Center();

  // Angles are in radians
  const TFloat angleToPlayer = atan2(playerCenter.y - p.y, playerCenter.x - p.x);

  TInt attackType = Random() & TUint8(3);

  p.Offset(-16, 16);
  // 25% chance for multi-arrow attack
  if (attackType < 3) {
    mGameState->AddProcess(new GEnemyCrystalProcess(mGameState, p.x, p.y, angleToPlayer, PROJECTILE_CRYSTAL_SLOT));
  } else {
    const TFloat step = 22.5 * (M_PI/180);
    const TFloat angles[3] = { angleToPlayer, angleToPlayer + step, angleToPlayer - step };
    mGameState->AddProcess(new GEnemyCrystalProcess(mGameState, p.x, p.y, angles[0], PROJECTILE_CRYSTAL_SLOT));
    mGameState->AddProcess(new GEnemyCrystalProcess(mGameState, p.x, p.y, angles[1], PROJECTILE_CRYSTAL_SLOT));
    mGameState->AddProcess(new GEnemyCrystalProcess(mGameState, p.x, p.y, angles[2], PROJECTILE_CRYSTAL_SLOT));
  }

  NewState(IDLE_STATE, DIRECTION_DOWN);
}

void GTurretProcess::Hit(DIRECTION aDirection) {
  mStateTimer = 0;
  StartBlink(BLINK_TIME);
}

void GTurretProcess::Death(DIRECTION aDirection) {
  mSprite->ClearFlags(SFLAG_ANIMATE);
}

TBool GTurretProcess::HitState() {
  mSprite->ClearFlags(SFLAG_ANIMATE);

  return GGruntProcess::HitState();
}

TBool GTurretProcess::MaybeAttack() {
  if (mGameState->IsGameOver() || GPlayer::mInvulnerable) {
    return EFalse;
  }

  const TFloat distance = sqrt(pow(GPlayer::mSprite->x - mSprite->x, 2) + pow(GPlayer::mSprite->y - mSprite->y, 2));

#ifdef DEBUG_MODE
  // printf("Distance from sniper to player %f\n", distance);
#endif

  if (distance > ATTACK_DISTANCE) {
    return EFalse;
  }

  TRect myRect, hisRect;
  mSprite->GetRect(myRect);
  GPlayer::mSprite->GetRect(hisRect);

  if (ABS(myRect.y2 - hisRect.y2) < ABS(myRect.x1 - hisRect.x1)) {
    if (myRect.x1 >= hisRect.x2) {
      // to right of player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_LEFT);
      }
      return ETrue;
    }
    if (myRect.x2 <= hisRect.x1) {
      // to left of player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_RIGHT);
      }
      return ETrue;
    }
  } else {
    if (myRect.y1 >= hisRect.y2) {
      // below player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_UP);
      }
      return ETrue;
    }
    if (myRect.y2 <= hisRect.y1) {
      // above player
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_DOWN);
      }
      return ETrue;
    }
  }

  mAttackTimer = 1;
  return EFalse;
}

TBool GTurretProcess::RunBefore() {
  mSprite->ResetShadow();

  if (--mShimmerTimer < 0) {
    if (++mSpriteImageNumber > 3) {
      mSpriteImageNumber = 0;
      mShimmerTimer = SHIMMER_TIMER * 6;
    } else {
      mShimmerTimer = SHIMMER_TIMER;
    }
  }
  mSprite->mImageNumber = mSpriteImageNumber;

  return GGruntProcess::RunBefore();
}
