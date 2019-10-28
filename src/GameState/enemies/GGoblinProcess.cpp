#include "GGoblinProcess.h"

#define DEBUGME
#undef DEBUGME

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt IDLE_SPEED = 5 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 2 * FACTOR;
const TInt HIT_SPEED = 1 * FACTOR;
const TInt WALK_SPEED = 5 * FACTOR;
const TInt DEATH_SPEED = 5 * FACTOR;

const TFloat VELOCITY = 1.5 / FACTOR;

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
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ASTEP(40, IMG_GOBLIN_IDLE),
  ASTEP(4, IMG_GOBLIN_IDLE + 1),
  ASTEP(40, IMG_GOBLIN_IDLE + 2),
  ASTEP(4, IMG_GOBLIN_IDLE + 1),
  ALOOP
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_GOBLIN_SELECTED + 2),
  ALOOP
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_GOBLIN_WALK_DOWN + 0),
  AEND
};

/*
 ____
|  _ \  _____      ___ __
| | | |/ _ \ \ /\ / / '_ \
| |_| | (_) \ V  V /| | | |
|____/ \___/ \_/\_/ |_| |_|
*/

static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 1),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 0),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 1),
  AEND
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 2),
  ADELTA(0, 1),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_DOWN + 3),
  AEND
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 3),
  ADELTA(0, 8),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 0),
  ADELTA(0, 8),
  ADELTA(0, 20),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 1),
  ASIZE(0, 16, 32, 32),
  ADELTA(0, 18),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 2),
  ATYPE(STYPE_ENEMY),
  ADELTA(0, 0),
  ASIZE(0, 0, 32, 16),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_DOWN + 0),
  AEND
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_DOWN + 3),
  AEND
};

/*
 _          __ _
| |    ___ / _| |_
| |   / _ \ |_| __|
| |__|  __/  _| |_
|_____\___|_|  \__|

 */

static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ADELTA(1, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 1),
  ADELTA(0, 0),
  AEND
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(4, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 2),
  ADELTA(2, 0),
  AFLIP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 3),
  ADELTA(0, 0),
  AEND
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),//
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ADELTA(-10, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(-10, 0, 40, 16),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ADELTA(-10, 0),
  AFLIP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ASIZE(0, 0, 32, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(-10, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  AFLIP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 0),
  AEND
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 0),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 3),
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
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 0),
  ADELTA(2, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 1),
  ADELTA(0, 0),
  AEND
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(-1, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 2),
  ADELTA(2, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_RIGHT + 3),
  ADELTA(0, 0),
  AEND
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),//
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 0),
  ADELTA(0, 0),
  ATYPE(STYPE_EBULLET),
  ASIZE(0, 0, 48, 16),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 1),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_RIGHT + 2),
  ASIZE(0, 0, 32, 16),
  ATYPE(STYPE_ENEMY),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 3),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 2, IMG_GOBLIN_ATTACK_RIGHT + 0),
  AEND
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_RIGHT + 3),
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
  ABITMAP(GOBLIN_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_GOBLIN_IDLE + 2),
  ALOOP
};


static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 0),
  ADELTA(-2, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 1),
  AEND
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(-4, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 2),
  ADELTA(-4, 0),
  ASTEP(WALK_SPEED, IMG_GOBLIN_WALK_UP + 3),
  AEND
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED * 4, IMG_GOBLIN_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ADELTA(-6, 0),
  ASIZE(0, 0, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 2),
  ADELTA(-2, 0),
  ASIZE(0, 0, 32, 32),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 1),
  ADELTA(-3, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_ATTACK_UP + 3),
  ATYPE(STYPE_ENEMY),
  ASIZE(0, 0, 32, 16),
  ADELTA(0, 0),
  ASTEP(ATTACK_SPEED, IMG_GOBLIN_WALK_UP + 0),
  AEND
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(GOBLIN_SLOT),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_GOBLIN_DAMAGE_UP + 3),
  AEND
};

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GGoblinProcess::GGoblinProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams)
  : GEnemyProcess(aGameState, aIp, GOBLIN_SLOT, aParams, VELOCITY) {
  mSprite->Name("GOBLIN SPRITE");
  mSprite->x = aX;
  mSprite->y = aY;
  mStartX = mSprite->x = aX;
  mStartY = mSprite->y = aY;
  mSprite->mHitStrength = HIT_MEDIUM;

  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GGoblinProcess::~GGoblinProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GGoblinProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
}

void GGoblinProcess::Walk(DIRECTION aDirection) {
  mSprite->vx = 0;
  mSprite->vy = 0;
  if (mStateTimer <= 0) {
    mStateTimer = TInt16(TFloat(Random(3, 8)) * 32 / VELOCITY);
  }
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
      mSprite->vy = -VELOCITY;
      break;
    case DIRECTION_DOWN:
      mSprite->vy = VELOCITY;
      mSprite->StartAnimation(
        mStep ? walkDownAnimation1 : walkDownAnimation2);
      break;
    case DIRECTION_LEFT:
      mSprite->vx = -VELOCITY;
      mSprite->StartAnimation(
        mStep ? walkLeftAnimation1 : walkLeftAnimation2);
      break;
    case DIRECTION_RIGHT:
      mSprite->vx = VELOCITY;
      mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
      break;
  }
}

void GGoblinProcess::Attack(DIRECTION aDirection) {
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(attackUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(attackDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(attackLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(attackRightAnimation);
      break;
  }
}

void GGoblinProcess::Hit(DIRECTION aDirection) {
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(hitUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(hitDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(hitLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(hitRightAnimation);
      break;
  }
}

void GGoblinProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}

