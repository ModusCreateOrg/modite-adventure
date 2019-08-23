#include "GSpiderProcess.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt   HIT_POINTS   = 5;
const TInt16 IDLE_TIMEOUT = 30;

const TInt IDLE_SPEED   = 5;
const TInt SELECT_SPEED = 5;
const TInt ATTACK_SPEED = 5;
const TInt HIT_SPEED    = 5;
const TInt WALK_SPEED   = 5;
const TInt DEATH_SPEED  = 5;

const TFloat VELOCITY = 1.5;
const TFloat SEEK_Y   = 6;  // seek to player Y within this many pixels
const TFloat SEEK_X   = 32;  // seek to player X within this many pixels

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
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(40, IMG_SPIDER_IDLE),
  ASTEP(4, IMG_SPIDER_IDLE + 1),
  ASTEP(40, IMG_SPIDER_IDLE + 2),
  ASTEP(4, IMG_SPIDER_IDLE + 1),
  ALOOP
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_SPIDER_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_SPIDER_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_SPIDER_SELECTED + 2),
  ALOOP
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(DEATH_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_SPIDER_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_SPIDER_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_SPIDER_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_SPIDER_WALK_DOWN + 0),
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
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 1),
  AEND
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_DOWN + 3),
  AEND
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_DOWN + 3),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_DOWN + 0),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_DOWN + 1),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_DOWN + 2),
  ATYPE(STYPE_ENEMY),
  AEND
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_DOWN + 3),
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
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ATYPE(STYPE_EBULLET),
  AFLIP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 3),
  AFLIP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 0),
  AFLIP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 1),
  AFLIP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  AEND
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 0),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
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
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 3),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 0),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 1),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_RIGHT + 2),
  ATYPE(STYPE_ENEMY),
  AEND
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_RIGHT + 3),
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
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 2),
  ALOOP
};


static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 1),
  AEND
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_SPIDER_WALK_UP + 3),
  AEND
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_UP + 3),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_UP + 0),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_UP + 1),
  ASTEP(ATTACK_SPEED, IMG_SPIDER_ATTACK_UP + 2),
  ATYPE(STYPE_ENEMY),
  AEND
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_SPIDER_DAMAGE_UP + 3),
  AEND
};

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GSpiderProcess::GSpiderProcess(GGameState *aGameState, GGamePlayfield *aGamePlayfield, TFloat aX, TFloat aY)
  : GEnemyProcess(aGameState, aGamePlayfield, SPIDER_SLOT) {
  mSprite->x          = aX;
  mSprite->y          = aY;
  mSprite->mHitPoints = HIT_POINTS;

  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GSpiderProcess::~GSpiderProcess() {
  //
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GSpiderProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;
  mSprite->mDx        = 0;
  mSprite->mDy        = 0;
  switch (aState) {
    case IDLE_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStateTimer = IDLE_TIMEOUT;
      break;

    case WALK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      if (mStateTimer <= 0) {
        mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
      }

      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mStep = 1 - mStep;
          mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
          mSprite->vy = -VELOCITY;
          break;
        case DIRECTION_DOWN:
          mStep = 1 - mStep;
          mSprite->vy = VELOCITY;
          mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
          break;
        case DIRECTION_LEFT:
          mStep = 1 - mStep;
          mSprite->vx = -VELOCITY;
//          mSprite->mDx = -36;
          mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
          break;
        case DIRECTION_RIGHT:
          mStep = 1 - mStep;
          mSprite->vx = VELOCITY;
          mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
          break;
      }
      break;
    case ATTACK_STATE:
      mAttackTimer = Random(30, 60);
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
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
      break;
    case HIT_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
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
      break;
    default:
      break;
  }
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

TBool GSpiderProcess::MaybeAttack() {
  if (abs(mPlayerSprite->y - mSprite->y) < SEEK_Y) {
    if (abs(mPlayerSprite->x - mSprite->x) <= SEEK_X + 16) {
      if (--mAttackTimer <= 0) {
        mAttackTimer = Random(30, 60);
        NewState(ATTACK_STATE, mPlayerSprite->x > mSprite->x ? DIRECTION_RIGHT : DIRECTION_LEFT);
        return ETrue;
      }
    }
  }

  return EFalse;
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

TBool GSpiderProcess::MaybeHit() {
  if (mSprite->cType & (STYPE_PLAYER | STYPE_PBULLET)) {
    if (mSprite->cType & STYPE_PBULLET) {
      printf("Spider ATTACKED\n");
      if (--mSprite->mHitPoints <= 0) {
        printf("Spider DEAD\n");
        mSprite->StartAnimation(deathAnimation);
        mState = DEATH_STATE;
        return ETrue;
      }
    }
    GAnchorSprite *other = mPlayerSprite;
    if ((mSprite->cType & STYPE_PBULLET) == 0) {
      mSprite->cType &= ~STYPE_PLAYER;
      if (mSprite->vx) {
        mSprite->x = other->x > mSprite->x ? other->x - 32 : other->x + 32;
        return EFalse;
      }
      if (mSprite->vy) {
        mSprite->y = other->y > mSprite->y ? other->y - 6 : other->y + 6;
        return EFalse;
      } else {
        mSprite->x = other->x > mSprite->x ? other->x - 32 : other->x + 32;
      }
    }

    mSprite->cType &= ~STYPE_PBULLET;
    switch (other->mDirection) {
      case DIRECTION_RIGHT:
        NewState(HIT_STATE, DIRECTION_LEFT);
        break;
      case DIRECTION_LEFT:
        NewState(HIT_STATE, DIRECTION_RIGHT);
        break;
      case DIRECTION_UP:
        NewState(HIT_STATE, DIRECTION_DOWN);
        break;
      case DIRECTION_DOWN:
        NewState(HIT_STATE, DIRECTION_UP);
        break;
    }
    return ETrue;
  }

  return EFalse;
}


TBool GSpiderProcess::IdleState() {
  if (MaybeAttack()) {
    return ETrue;
  }
  if (MaybeHit()) {
    return ETrue;
  }
  if (--mStateTimer < 0) {
    if (mSprite->Clipped()) {
      NewState(IDLE_STATE, mSprite->mDirection);
      return ETrue;
    }
    if (abs(mPlayerSprite->y - mSprite->y) > SEEK_Y) {
      NewState(WALK_STATE, mPlayerSprite->y - mSprite->y > 0 ? DIRECTION_DOWN : DIRECTION_UP);
      return ETrue;
    } else if (abs(mPlayerSprite->x - mSprite->x) > SEEK_X) {
      NewState(WALK_STATE, mPlayerSprite->x - mSprite->x > 0 ? DIRECTION_RIGHT : DIRECTION_LEFT);
      return ETrue;
    }
    NewState(IDLE_STATE, mPlayerSprite->x < mSprite->x ? DIRECTION_LEFT : DIRECTION_RIGHT);
  }
  return ETrue;
}

TBool GSpiderProcess::WalkState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (mSprite->Clipped()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }
  TFloat screenX = mSprite->x - mGameState->mWorldXX,
         screenY = mSprite->y - mGameState->mWorldYY;

  if (mPlayfield->IsWall(mSprite->x + 16 + mSprite->vx, mSprite->y + mSprite->vy) ||      // Left/Bottom Wall
      mPlayfield->IsWall(mSprite->x + 16 + mSprite->vx, mSprite->y - 32 + mSprite->vy) || // Left/Top Wall
      mPlayfield->IsWall(mSprite->x + 48 + mSprite->vx, mSprite->y + mSprite->vy) ||      // Right/Bottom Wall
      mPlayfield->IsWall(mSprite->x + 48 + mSprite->vx, mSprite->y - 32 + mSprite->vy) || // Right/Top Wall
      screenX < 16 || screenX > (SCREEN_WIDTH - 16) || screenY < 16 || screenY > (SCREEN_HEIGHT - 16)
    ) {
    NewState(IDLE_STATE, mPlayerSprite->x < mSprite->x ? DIRECTION_LEFT : DIRECTION_RIGHT);
    return ETrue;
  }
  if (abs(mPlayerSprite->y - mSprite->y) > SEEK_Y) {
    // keep walking
    if (mSprite->AnimDone()) {
      NewState(WALK_STATE, mPlayerSprite->y - mSprite->y > 0 ? DIRECTION_DOWN : DIRECTION_UP);
    }
    return ETrue;
  } else if (abs(mPlayerSprite->x - mSprite->x) > SEEK_X + 2) {
    // keep walking
    if (mSprite->AnimDone()) {
      NewState(WALK_STATE, mPlayerSprite->x - mSprite->x > 0 ? DIRECTION_RIGHT : DIRECTION_LEFT);
    }
    return ETrue;
  }

  // next to player
  NewState(IDLE_STATE, mPlayerSprite->x - mSprite->x > 0 ? DIRECTION_RIGHT : DIRECTION_LEFT);

//  if (mSprite->AnimDone()) {
//    NewState(WALK_STATE, mSprite->mDirection);
//  }

  return ETrue;
}

TBool GSpiderProcess::AttackState() {
  if (mSprite->AnimDone()) {
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GSpiderProcess::HitState() {
  if (mSprite->AnimDone()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    mSprite->cType &= STYPE_PLAYER;
  }

  return ETrue;
}

TBool GSpiderProcess::DeathState() {
  if (mSprite->AnimDone()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    mSprite->cType &= STYPE_PLAYER | STYPE_PBULLET;
    mSprite->mHitPoints = HIT_POINTS;
  }

  return ETrue;
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

TBool GSpiderProcess::RunBefore() {
  switch (mState) {
    case IDLE_STATE:
      return IdleState();
    case WALK_STATE:
      return WalkState();
    case ATTACK_STATE:
      return AttackState();
    case HIT_STATE:
      return HitState();
    case DEATH_STATE:
      return DeathState();
    default:
      return ETrue;
  }
}

TBool GSpiderProcess::RunAfter() {
  return ETrue;
}

