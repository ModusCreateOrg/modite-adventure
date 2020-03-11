#include "GTurretProcess.h"
#include "GEnemyProjectileProcess.h"
#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt IDLE_SPEED = 6 * FACTOR;
const TInt HIT_SPEED = 1 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = 4 / FACTOR;
const TFloat ATTACK_DISTANCE = 6 * TILESIZE;
const TInt MAX_HIT_STATE = 4;

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
    : GEnemyProcess(aGameState, aIp, TURRET_SLOT, aParam, VELOCITY, ATTR_TURRET) {
  mGameState = aGameState;

  mSprite->Name("ENEMY TURRET");
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->w = 22;
  mSprite->h = 16;
  mSprite->cx = 10;
  mSprite->cy = 16;
  mStartX = mSprite->x = aX;
  mStartY = mSprite->y = aY;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CRYSTAL_BMP_SPRITES);
  mSprite->vx = mSprite->vy = 0;

  mHitState = 0;
  mTaunt = EFalse;
  NewState(IDLE_STATE, DIRECTION_DOWN);

  mSprite->SetStatMultipliers(5.0, 10.0, 5.0);
  mSprite->StartAnimation(idleAnimation);
}

GTurretProcess::~GTurretProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

void GTurretProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  const TInt lastDx = mSprite->mDx;
  const TInt lastDy = mSprite->mDy;
  GEnemyProcess::NewState(aState, aDirection);
  mSprite->mDx = lastDx;
  mSprite->mDy = lastDy;
}

void GTurretProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  if (mSprite->AnimDone()) {
    mSprite->StartAnimation(idleAnimation);
  }
}

void GTurretProcess::Taunt(DIRECTION aDirection) {
  return;
}

void GTurretProcess::Walk(DIRECTION aDirection) {
  return;
}

void GTurretProcess::Attack(DIRECTION aDirection) {
  // fire 1-3 projectiled
  TFloat xx = mSprite->x,
         yy = mSprite->y;

  // Angles are in radians
  const TFloat angleToPlayer = atan2(GPlayer::mSprite->y - yy, GPlayer::mSprite->x - xx);

  TInt attackType = Random() & TUint8(3);

  // 25% chance for multi-arrow attack
  if (attackType < 3) {
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 8, angleToPlayer, ENVIRONMENT_SLOT, IMG_CRYSTAL_PROJECTILE));
  } else {
    const TFloat step = 22.5 * (M_PI/180);
    const TFloat angles[3] = { angleToPlayer, angleToPlayer + step, angleToPlayer - step };
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 8, angles[0], ENVIRONMENT_SLOT, IMG_CRYSTAL_PROJECTILE));
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 8, angles[1], ENVIRONMENT_SLOT, IMG_CRYSTAL_PROJECTILE));
    mGameState->AddProcess(new GEnemyProjectileProcess(mGameState, xx + 16, yy - 8, angles[2], ENVIRONMENT_SLOT, IMG_CRYSTAL_PROJECTILE));
  }

  NewState(IDLE_STATE, DIRECTION_DOWN);
}

void GTurretProcess::Hit(DIRECTION aDirection) {
  mStateTimer = 0;
}

void GTurretProcess::Death(DIRECTION aDirection) {
  mSprite->ClearFlags(SFLAG_ANIMATE);
}

TBool GTurretProcess::HitState() {
  if (--mStateTimer < 0) {
    if (mHitState % 2 == 0) {
      mSprite->mFill = -1;
    } else {
      mSprite->mFill = COLOR_WHITE;
    }
    mStateTimer = HIT_SPEED;
    mHitState++;
  }

  if (mHitState > MAX_HIT_STATE) {
    mHitState = 0;
    mSprite->ClearFlags(SFLAG_ANIMATE);
  }

  return GEnemyProcess::HitState();
}

TBool GTurretProcess::MaybeAttack() {
  if (mPlayerSprite->mInvulnerable) {
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
  mPlayerSprite->GetRect(hisRect);

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
  return GEnemyProcess::RunBefore();
}
