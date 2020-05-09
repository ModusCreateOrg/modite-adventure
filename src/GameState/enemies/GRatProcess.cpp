#include "GRatProcess.h"
#include "GStatProcess.h"
//#include "GPlayer.h"

#define DEBUGME
#undef DEBUGME

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 10 * FACTOR;

const TInt16 IDLE_SPEED = 5 * FACTOR;
const TInt16 SELECT_SPEED = 5 * FACTOR;
const TInt TAUNT_SPEED = 5 * FACTOR;
const TInt16 ATTACK_SPEED = 2 * FACTOR;
const TInt16 HIT_SPEED = 1 * FACTOR;
const TInt16 WALK_SPEED = 2 * FACTOR;
const TInt16 DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = PLAYER_VELOCITY * 1.5;

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

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(RAT_SLOT),
  ALABEL,
  ASTEP(40, IMG_RAT_IDLE),
  ASTEP(4, IMG_RAT_IDLE + 1),
  ASTEP(40, IMG_RAT_IDLE + 2),
  ASTEP(4, IMG_RAT_IDLE + 1),
  ALOOP,
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(RAT_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_RAT_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_RAT_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_RAT_SELECTED + 2),
  ALOOP,
};

static ANIMSCRIPT tauntAnimation[] = {
  ABITMAP(RAT_SLOT),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 0),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 6),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 5),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 4),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 2),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 1),
  ASTEP(TAUNT_SPEED, IMG_RAT_IDLE + 0),
  AEND,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(RAT_SLOT),
  ASTEP(DEATH_SPEED, IMG_RAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_RAT_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_RAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_RAT_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_RAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_RAT_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_RAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_RAT_WALK_DOWN + 0),
  ASTEP(1, IMG_RAT_WALK_UP + 0),
  ANULL(1),
  ASTEP(1, IMG_RAT_WALK_UP + 0),
  ANULL(1),
  ASTEP(1, IMG_RAT_WALK_UP + 0),
  ANULL(1),
  ASTEP(1, IMG_RAT_WALK_UP + 0),
  ANULL(1),
  ASTEP(1, IMG_RAT_WALK_UP + 0),
  ANULL(1),
  AEND,
};

/*
 ____
|  _ \  _____      ___ __
| | | |/ _ \ \ /\ / / '_ \
| |_| | (_) \ V  V /| | | |
|____/ \___/ \_/\_/ |_| |_|
*/

static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(RAT_SLOT),
  ALABEL,
  ADELTA(0, 8),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_DOWN + 0),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(0, 8),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(0, 8),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(0, 8),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_DOWN + 3),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 16, 32, 40),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_DOWN + 1),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_DOWN + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 4, 32, 24),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_DOWN + 0),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(RAT_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 3),
  AFILL(-1),
  AEND,
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */

static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(RAT_SLOT),
  ALABEL,
  ADELTA(8, 0),
  AFLIP(1, IMG_RAT_WALK_RIGHT + 0),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(8, 0),
  AFLIP(WALK_SPEED, IMG_RAT_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_RAT_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(8, 0),
  AFLIP(WALK_SPEED, IMG_RAT_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_RAT_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(8, 0),
  AFLIP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 3),
  AFLIP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-24, 4, 48, 24),
  AFLIP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 1),
  AFLIP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 4, 32, 24),
  AFLIP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(RAT_SLOT),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 1),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AEND,
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
  ABITMAP(RAT_SLOT),
  ALABEL,
  ADELTA(-8, 0),
  ASTEP(1, IMG_RAT_WALK_RIGHT + 0),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(-8, 0),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(-8, 0),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(-8, 0),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 3),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-8, 4, 48, 24),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 1),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 4, 32, 24),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_RIGHT + 0),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(RAT_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AEND,
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
  ABITMAP(RAT_SLOT),
  ALABEL,
  ADELTA(0, 12),
  ASTEP(1, IMG_RAT_WALK_UP + 0),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(0, 12),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(0, 12),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_RAT_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(RAT_SLOT),
  ADELTA(0, 12),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_UP + 3),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, -16, 32, 40),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_UP + 1),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_UP + 2),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 4, 32, 24),
  ASTEP(ATTACK_SPEED, IMG_RAT_ATTACK_UP + 0),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(RAT_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_UP + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_UP + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_UP + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_UP + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_UP + 3),
  AFILL(-1),
  AEND,
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(RAT_SLOT),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_RAT_DAMAGE_DOWN + 3),
  AEND,
};

static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};

// endregion }}}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GRatProcess::GRatProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GEnemyProcess(aGameState, aIp, RAT_SLOT, aParams, VELOCITY, ATTR_RAT) {
  mSprite->Name("ENEMY RAT");
  mStartX = mSprite->x = aX;
  mStartY = mSprite->y = aY;
  mSprite->SetStatMultipliers(1.5, 1.5, 1.0);
  mSprite->h = 24;
  mStateTimer = 0;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_RAT_BMP_SPRITES);
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GRatProcess::~GRatProcess() = default;

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

TBool GRatProcess::CanWalkInDirection(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
  // force follow walls
  if (!mSprite->CanWalk(aVx, aVy)) {
    return EFalse;
  }
  if (IsWallInDirection(GAnchorSprite::RotateDirection(aDirection, 1))) {
    return ETrue;
  }
  if (IsWallInDirection(GAnchorSprite::RotateDirection(aDirection, 3))) {
    return ETrue;
  }
  // no walls at all?  Move to the nearest one.
  if (!IsWallInDirection(GAnchorSprite::RotateDirection(aDirection, 2))) {
    mStateTimer++;
    return ETrue;
  }

  return EFalse;
}

void GRatProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  mSprite->StartAnimationInDirection(idleAnimations, aDirection);
}

void GRatProcess::Taunt(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(tauntAnimation);
}

void GRatProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(5, 10)) * 32 / VELOCITY);
  }
  mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
  mSprite->MoveInDirection(VELOCITY, aDirection);
}

void GRatProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(attackAnimations, aDirection);
}

void GRatProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(hitAnimations, aDirection);
}

void GRatProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitSpellAnimation);
}

void GRatProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}
