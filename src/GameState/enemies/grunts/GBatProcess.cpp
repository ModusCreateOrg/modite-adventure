#include "GBatProcess.h"
#include "GResources.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 5 * FACTOR;
const TInt16 LAND_TIMEOUT = 150 * FACTOR;

const TInt IDLE_SPEED = 3 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 3 * FACTOR;
const TInt HIT_SPEED = 2 * FACTOR;
const TInt WALK_SPEED = 2 * FACTOR;
const TInt DEATH_SPEED = 2 * FACTOR;

const TFloat VELOCITY = 1.5 / TFloat(FACTOR);
const TInt MAX_ALTITUDE = 8;

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
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(40, IMG_BAT_IDLE),
  ASTEP(4, IMG_BAT_IDLE + 1),
  ASTEP(40, IMG_BAT_IDLE + 2),
  ASTEP(4, IMG_BAT_IDLE + 1),
  ALOOP,
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_BAT_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_BAT_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_BAT_SELECTED + 2),
  ALOOP,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_BAT_WALK_DOWN + 0),
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
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_DOWN + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_DOWN + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_DOWN + 2),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_DOWN + 3),
  ALOOP,
};

static ANIMSCRIPT landedDownAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  ALOOP,
};

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
  ASTEP(ATTACK_SPEED * 2, IMG_BAT_ATTACK_DOWN + 3),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0,12,32,24),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 1),
  ASIZE(0,4,32,16),
  ATYPE(STYPE_ENEMY),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_DOWN + 2),
  AEND,
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
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
  ABITMAP(BAT_SLOT),
  ALABEL,
  AFLIP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 0),
  AFLIP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 1),
  AFLIP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 2),
  AFLIP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 3),
  ALOOP,
};

static ANIMSCRIPT landedLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  AFLIP(IDLE_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  ALOOP,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(BAT_SLOT),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(BAT_SLOT),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 3),
  AEND,
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 3),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-12,4,40,16),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  AFLIP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 2),
  AEND,
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 1),
  AFILL(-1),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  AFLIP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
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
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 2),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_RIGHT + 3),
  ALOOP,
};

static ANIMSCRIPT landedRightAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  ALOOP,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_RIGHT + 0),
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
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 3),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-4,4,40,16),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASIZE(0,4,32,16),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_RIGHT + 2),
  AEND,
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_RIGHT + 3),
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
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_UP + 0),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_UP + 1),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_UP + 2),
  ASTEP(IDLE_SPEED, IMG_BAT_WALK_UP + 3),
  ALOOP,
};

static ANIMSCRIPT landedUpAnimation[] = {
  ABITMAP(BAT_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_BAT_DAMAGE_UP + 3),
  ALOOP,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_BAT_WALK_UP + 3),
  AEND,
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 3),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0,4,32,24),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 1),
  ASIZE(0,4,32,16),
  ATYPE(STYPE_ENEMY),
  ASTEP(ATTACK_SPEED, IMG_BAT_ATTACK_UP + 2),
  AEND,
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(BAT_SLOT),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 3),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 0),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 1),
  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 2),
  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_UP + 3),
  AFILL(-1),
  AEND,
};

static ANIMSCRIPT hitSpellAnimation[] = {
  ABITMAP(BAT_SLOT),
  ASTEP(HIT_SPEED, IMG_BAT_DAMAGE_DOWN + 3),
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
  AEND,
};

static ANIMSCRIPT* idleAnimations[] = {idleUpAnimation, idleDownAnimation, idleLeftAnimation, idleRightAnimation};
static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* attackAnimations[] = {attackUpAnimation, attackDownAnimation, attackLeftAnimation, attackRightAnimation};
static ANIMSCRIPT* hitAnimations[] = {hitUpAnimation, hitDownAnimation, hitLeftAnimation, hitRightAnimation};
static ANIMSCRIPT* landedAnimations[] = {landedUpAnimation, landedDownAnimation, landedLeftAnimation, landedRightAnimation};

/* endregion }}} */

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GBatProcess::GBatProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
    : GGruntProcess(aGameState, aX, aY, aIp, BAT_SLOT, aParams, VELOCITY, ATTR_BAT) {
  mStateTimer = 0;
  mLandTimer = 0;
  mAltitude = MAX_ALTITUDE;
  mSprite->Name("ENEMY BAT");
  SetStatMultipliers(1.8, 2.0, 1.0);
  mSprite->w = 24;
  mSprite->cx = 8;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_BAT_BMP_SPRITES);

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

void GBatProcess::Land(DIRECTION aDirection) {
  mSprite->StartAnimationInDirection(landedAnimations, aDirection);
}

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
  mSprite->StartAnimation(deathAnimation);
}

void GBatProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  if (mState != IDLE_STATE && aState == IDLE_STATE) {
    if (!Random(0, 2)) {
      mLandTimer = LAND_TIMEOUT;
    }
    mStateTimer = IDLE_TIMEOUT;
  }
  if (mState != IDLE_STATE || aState != IDLE_STATE) {
    GGruntProcess::NewState(aState, aDirection);
  } else {
    if (--mLandTimer > 0) {
      mAltitude = MIN(mAltitude, MAX_ALTITUDE);
      if (mAltitude > 0) {
        mAltitude -= GRAVITY;
      } else {
        mAltitude = 0;
        Land(aDirection);
      }
    } else {
      if (mLandTimer == 0) {
        Idle(aDirection);
      }
      if (mAltitude < MAX_ALTITUDE) {
        mAltitude += GRAVITY;
      }
    }
  }

  mSprite->mDy = 16 - TInt(mAltitude);
}

TBool GBatProcess::CanWalkInDirection(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
  if (mState == IDLE_STATE && (mAltitude < MAX_ALTITUDE || mLandTimer > 0)) {
    return EFalse;
  }

  return GGruntProcess::CanWalkInDirection(aDirection, aVx, aVy);
}
