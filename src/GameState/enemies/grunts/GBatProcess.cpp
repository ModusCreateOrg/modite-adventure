#include "GBatProcess.h"
#include "GResources.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 5 * FACTOR;

const TInt IDLE_SPEED = 3 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 3 * FACTOR;
const TInt HIT_SPEED = 2 * FACTOR;
const TInt WALK_SPEED = 3 * FACTOR;
const TInt DEATH_SPEED = 1;

const TFloat VELOCITY = 1.05 * PLAYER_VELOCITY;

// region  ANIMATIONS {{{

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

/**
 * Animations
 */

/*
 ____
|  _ \  _____      ___ __
| | | |/ _ \ \ /\ / / '_ \
| |_| | (_) \ V  V /| | | |
|____/ \___/ \_/\_/ |_| |_|
*/

static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_DOWN + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_DOWN + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_DOWN + 2),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_DOWN + 3),
  ALOOP,
};

//static ANIMSCRIPT landedDownAnimation[] = {
//  ABITMAP(BAT_SLOT),
//  ALABEL,
//  ASTEP(IDLE_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
//  ALOOP,
//};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 14, 24, 42),
  ADELTA(0, 14),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 1),
  ADELTA(0, 12),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 0, 24, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 1),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT deathDownAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(DEATH_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  AEND,
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */

// Idle animations should seem like he's struggling to stay still.
static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  ADELTA(5, 0),
  AFLIP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ADELTA(4, 0),
  AFLIP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 1),
  ADELTA(-3, 0),
  AFLIP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 2),
  ADELTA(1, 0),
  AFLIP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 3),
  ALOOP,
};

//static ANIMSCRIPT landedLeftAnimation[] = {
//  ABITMAP(BAT_SLOT),
//  ALABEL,
//  AFLIP(IDLE_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
//  ALOOP,
//};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(5, 0),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ADELTA(4, 0),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(-3, 0),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 2),
  ADELTA(1, 0),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(9, 0),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-20, 0, 40, 24),
  ADELTA(-13, 0),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 1),
  ADELTA(-21, 0),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 0, 24, 16),
  ADELTA(-3, 0),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFILL(COLOR_WHITE),
  ADELTA(-6, 0),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 0),
  AFILL(-1),
  ADELTA(1, 0),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 1),
  AFILL(COLOR_WHITE),
  ADELTA(-3, 0),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 2),
  AFILL(-1),
  ADELTA(-1, 0),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT deathLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(-6, 0),
  AFLIP(DEATH_SPEED, IMG_BAT_DAMAGE_RIGHT + 0),
  AEND
};

/*
 ____  _       _     _
|  _ \(_) __ _| |__ | |_
| |_) | |/ _` | '_ \| __|
|  _ <| | (_| | | | | |_
|_| \_\_|\__, |_| |_|\__|
         |___/
 */

static ANIMSCRIPT idleRightAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ADELTA(-1, 0),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 2),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 3),
  ALOOP,
};

//static ANIMSCRIPT landedRightAnimation[] = {
//  ABITMAP(BAT_SLOT),
//  ALABEL,
//  ASTEP(IDLE_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
//  ALOOP,
//};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 0, 42, 24),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 1),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 0, 24, 16),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFILL(COLOR_WHITE),
  ADELTA(-1, 0),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 0),
  ADELTA(0, 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 1),
  ADELTA(0, 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 2),
  AFILL(-1),
  ADELTA(0, 0),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT deathRightAnimation[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(-1, 0),
  ASTEP(DEATH_SPEED, IMG_BAT_DAMAGE_RIGHT + 0),
  AEND
};

/*
 _   _
| | | |_ __
| | | | '_ \
| |_| | |_) |
 \___/| .__/
      |_|
 */

static ANIMSCRIPT idleUpAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_UP + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_UP + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_UP + 2),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_UP + 3),
  ALOOP,
};

//static ANIMSCRIPT landedUpAnimation[] = {
//  ABITMAP(BAT_SLOT),
//  ALABEL,
//  ASTEP(IDLE_SPEED, IMG_BAT_DAMAGE_UP + 3),
//  ALOOP,
//};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 0),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(BAT_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 2),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(8, 0, 24, 42),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 1),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(8, 0, 24, 16),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 3),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 0),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 1),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 2),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 3),
  AEND,
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 0),
  AEND,
};

static ANIMSCRIPT deathUpAnimation[] = {
    ABITMAP(BAT_SLOT),
    ASTEP(DEATH_SPEED, IMG_BAT_DAMAGE_UP + 0),
    AEND
};

static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};
static ANIMSCRIPT* deathAnimations[] = {deathUpAnimation, deathDownAnimation, deathLeftAnimation, deathRightAnimation};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GBatProcess::GBatProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GGruntProcess(aGameState, aIp, aX, aY, BAT_SLOT, aParams, VELOCITY, ATTR_BAT) {
  mStateTimer = 0;
//  mIdleTimer = 0;
//  mAltitude = MAX_ALTITUDE;
  mSprite->Name("ENEMY BAT");
  SetStatMultipliers(1.8, 2.0, 1.0);
  mSprite->w = 24;
  mSprite->h = 16;
  mSprite->cx = 8;
  mSprite->cy = 4;
  mSprite->ResetShadow();
  mSprite->ClearFlags(SFLAG_RENDER_SHADOW);
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_BAT_BMP_SPRITES);
  mRangeX = mRangeY = 16;

  mTaunt = EFalse; // bat does not taunt
}

GBatProcess::~GBatProcess() = default;

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GBatProcess::Idle(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(idleAnimations, aDirection);
}

void GBatProcess::Taunt(DIRECTION aDirection) {
  // Bat does not taunt
}

//void GBatProcess::Land(DIRECTION aDirection) {
//  mSprite->StartAnimationInDirection(landedAnimations, aDirection);
//}

void GBatProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
  }
  mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
  mSprite->MoveInDirection(VELOCITY, aDirection);
}

void GBatProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(attackAnimations, aDirection);
}

void GBatProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitAnimations, aDirection);
}

void GBatProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitSpellAnimation);
}

void GBatProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(deathAnimations, aDirection);
}

//void GBatProcess::NewState(TUint16 aState, DIRECTION aDirection) {
//  if (mState != IDLE_STATE && aState == IDLE_STATE) {
//    if (!Random(0, 2)) {
//      mIdleTimer = LAND_TIMEOUT;
//    }
//    mStateTimer = IDLE_TIMEOUT;
//  }
//
//  if (mState != IDLE_STATE || aState != IDLE_STATE) {
//    GEnemyProcess::NewState(aState, aDirection);
//  } else {
//    if (--mIdleTimer > 0) {
//      Idle(aDirection);
//    }
//  }
//
////  mSprite->mDy = 16 - TInt(mAltitude);
//}

//TBool GBatProcess::CanWalkInDirection(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
//  if (mState == IDLE_STATE && (mAltitude < MAX_ALTITUDE || mIdleTimer > 0)) {
//    return EFalse;
//  }
//
//  return GEnemyProcess::CanWalkInDirection(aDirection, aVx, aVy);
//}
