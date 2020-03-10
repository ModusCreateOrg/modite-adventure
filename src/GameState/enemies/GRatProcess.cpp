#include "GRatProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"

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

ANIMSCRIPT idleAnimation[] = {
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

GRatProcess::~GRatProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

TBool GRatProcess::CanWalk(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
  // force follow walls
  switch (aDirection) {

    case DIRECTION_UP:
      if (!mSprite->CanWalk(aDirection, 0, aVy)) {
        return EFalse;
      }
      if (IsWall(DIRECTION_LEFT, 0, 0)) {
        return ETrue;
      }
      if (IsWall(DIRECTION_RIGHT, 0, 0)) {
        return ETrue;
      }
      // no walls at all?  Move ot the nearest one.
      if (!IsWall(DIRECTION_DOWN, 0, 0)) {
        // no walls at all?  Move ot the nearest one.
        mStateTimer++;
        return ETrue;
      }
      break;

      //
    case DIRECTION_DOWN:
      if (!mSprite->CanWalk(aDirection, 0, aVy)) {
        return EFalse;
      }
      // no wall above, assure there is a wall left or right
      if (IsWall(DIRECTION_LEFT, 0, 0)) {
        return ETrue;
      }
      if (IsWall(DIRECTION_RIGHT, 0, 0)) {
        return ETrue;
      }
      if (!IsWall(DIRECTION_UP, 0, 0)) {
        // no walls at all?  Move ot the nearest one.
        mStateTimer++;
        return ETrue;
      }
      break;

      //
    case DIRECTION_LEFT:
      if (!mSprite->CanWalk(aDirection, aVx, 0)) {
        return EFalse;
      }
      // no wall to left, assure there is a wall above or below
      if (IsWall(DIRECTION_UP, 0, 0)) {
        return ETrue;
      }
      if (IsWall(DIRECTION_DOWN, 0, 0)) {
        return ETrue;
      }
      if (!IsWall(DIRECTION_RIGHT, 0, 0)) {
        // no walls at all?  Move ot the nearest one.
        mStateTimer++;
        return ETrue;
      }
      break;

      //
    case DIRECTION_RIGHT:
      if (!mSprite->CanWalk(aDirection, aVx, 0)) {
        return EFalse;
      }
      // no wall to right, assure there is a wall above or below
      if (IsWall(DIRECTION_UP, 0, 0)) {
        return ETrue;
      }
      if (IsWall(DIRECTION_DOWN, 0, 0)) {
        return ETrue;
      }
      if (!IsWall(DIRECTION_LEFT, 0, 0)) {
        // no walls at all?  Move ot the nearest one.
        mStateTimer++;
        return ETrue;
      }
      break;

      //
    default:
      Panic("Rat CanWalk invalid direction %d\n", aDirection);
      break;
  }

  return EFalse;
}

void GRatProcess::Idle(DIRECTION aDirection) {
  mStateTimer = IDLE_TIMEOUT;
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(idleUpAnimation);
      break;
    case DIRECTION_DOWN:
      mSprite->StartAnimation(idleDownAnimation);
      break;
    case DIRECTION_LEFT:
      mSprite->StartAnimation(idleLeftAnimation);
      break;
    case DIRECTION_RIGHT:
      mSprite->StartAnimation(idleRightAnimation);
      break;
    default:
      Panic("GRatProcess no idle direction\n");
      break;
  }
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
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
      mSprite->vy = -VELOCITY;
      break;
    case DIRECTION_DOWN:
      mSprite->vy = VELOCITY;
      mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
      break;
    case DIRECTION_LEFT:
      mSprite->vx = -VELOCITY;
      mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
      break;
    case DIRECTION_RIGHT:
      mSprite->vx = VELOCITY;
      mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
      break;
    default:
      Panic("GRatProcess no walk direction\n");
      break;
  }
}

void GRatProcess::Attack(DIRECTION aDirection) {
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
    default:
      Panic("GRatProcess no Attack direction\n");
      break;
  }
}

void GRatProcess::Hit(DIRECTION aDirection) {
  switch (aDirection) {
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
    case DIRECTION_SPELL:
      mSprite->StartAnimation(hitSpellAnimation);
      break;
    default:
      Panic("GRatProcess no Hit direction\n");
      break;
  }
}

void GRatProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}
