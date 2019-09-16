#include "GOrcProcess.h"

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

const TInt   HIT_POINTS   = 5;
const TInt16 IDLE_TIMEOUT = 30 * FACTOR;

const TInt IDLE_SPEED   = 5 * FACTOR;
const TInt SELECT_SPEED = 5 * FACTOR;
const TInt ATTACK_SPEED = 5 * FACTOR;
const TInt HIT_SPEED    = 1 * FACTOR;
const TInt WALK_SPEED   = 5 * FACTOR;
const TInt DEATH_SPEED  = 5 * FACTOR;

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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(40, IMG_ORC_IDLE),
  ASTEP(4, IMG_ORC_IDLE + 1),
  ASTEP(40, IMG_ORC_IDLE + 2),
  ASTEP(4, IMG_ORC_IDLE + 1),
  ALOOP
};

static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_ORC_SELECTED + 2),
  ALOOP
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_DOWN + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_UP + 0),
  AFLIP(DEATH_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(DEATH_SPEED, IMG_ORC_WALK_DOWN + 0),
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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 1),
  AEND
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_DOWN + 3),
  AEND
};

static ANIMSCRIPT attackDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 3),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 0),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 1),
  ATYPE(STYPE_ENEMY),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_DOWN + 2),
  AEND
};

static ANIMSCRIPT hitDownAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_DOWN + 3),
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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(ORC_SLOT),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(ORC_SLOT),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 2),
  AFLIP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  AFLIP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  AEND
};

static ANIMSCRIPT hitLeftAnimation[] = {
  ABITMAP(ORC_SLOT),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  AFLIP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 2),
  ALOOP
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT attackRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 3),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 0),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 1),
  ATYPE(STYPE_ENEMY),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_RIGHT + 2),
  AEND
};

static ANIMSCRIPT hitRightAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_RIGHT + 3),
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
  ABITMAP(ORC_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_ORC_IDLE + 2),
  ALOOP
};


static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 1),
  AEND
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_ORC_WALK_UP + 3),
  AEND
};

static ANIMSCRIPT attackUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 3),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 0),
  ATYPE(STYPE_EBULLET),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 1),
  ATYPE(STYPE_ENEMY),
  ASTEP(ATTACK_SPEED, IMG_ORC_ATTACK_UP + 2),
  AEND
};

static ANIMSCRIPT hitUpAnimation[] = {
  ABITMAP(ORC_SLOT),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 0),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 1),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 2),
  ASTEP(HIT_SPEED, IMG_ORC_DAMAGE_UP + 3),
  AEND
};

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

// constructor
GOrcProcess::GOrcProcess(GGameState *aGameState, GGamePlayfield *aGamePlayfield, TFloat aX, TFloat aY)
  : GEnemyProcess(aGameState, aGamePlayfield, ORC_SLOT) {
  mSprite->Name("ORC SPRITE");
  mSprite->x          = aX;
  mSprite->y          = aY;
  mSprite->mHitPoints = HIT_POINTS;

  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GOrcProcess::~GOrcProcess() {
  //
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GOrcProcess::NewState(TUint16 aState, DIRECTION aDirection) {
#ifdef DEBUGME
  printf("ORC NewState(%d,%d) %s[mState]\n", aState, aDirection, stateMessages[aState]);
#endif
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
      mSprite->StartAnimation(selectAnimation);
      break;

    case WALK_STATE:
#ifdef DEBUGME
      printf("ORC WALK_STATE %d\n", mSprite->mDirection);
#endif
      mSprite->vx = 0;
      mSprite->vy = 0;
      if (mStateTimer <= 0) {
#ifdef DEBUGME
        printf("here\n");
#endif
        mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);
      }

      switch (mSprite->mDirection) {
        case DIRECTION_UP:
#ifdef DEBUGME
          printf("Start Walk Up %d\n", mStep);
#endif
          mStep = 1 - mStep;
          mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
          mSprite->vy = -VELOCITY;
          break;
        case DIRECTION_DOWN:
#ifdef DEBUGME
          printf("Start Walk Down %d\n", mStep);
#endif
          mStep = 1 - mStep;
          mSprite->vy = VELOCITY;
          mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
          break;
        case DIRECTION_LEFT:
#ifdef DEBUGME
          printf("Start Walk Left %d\n", mStep);
#endif
          mStep = 1 - mStep;
          mSprite->vx = -VELOCITY;
//          mSprite->mDx = -36;
          mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
          break;
        case DIRECTION_RIGHT:
#ifdef DEBUGME
          printf("Start Walk Right %d\n", mStep);
#endif
          mStep = 1 - mStep;
          mSprite->vx = VELOCITY;
          mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
          break;
      }
      break;
    case ATTACK_STATE:
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

TBool GOrcProcess::MaybeHit() {
  if (mSprite->cType & STYPE_PBULLET) {
    if (--mSprite->mHitPoints <= 0) {
      mSprite->StartAnimation(deathAnimation);
      mState = DEATH_STATE;
      return ETrue;
    }
  }

  if (mSprite->cType & (STYPE_PLAYER | STYPE_PBULLET)) {
    GAnchorSprite *other = mSprite->mCollided;
    mSprite->cType &= ~(STYPE_PLAYER | STYPE_PBULLET);
    mSprite->Nudge();
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

TBool GOrcProcess::IdleState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (mSprite->flags & SFLAG_CLIPPED) {
    return ETrue;
  }
  if (--mStateTimer < 0) {
    // Set distance to walk for WALK_STATE
    mStateTimer = TInt16(TFloat(Random(1, 3)) * 32 / VELOCITY);

    TFloat x  = mSprite->x,
           y  = mSprite->y,
           sx = x - mGameState->GetViewPort()->mWorldX,
           sy = y - mGameState->GetViewPort()->mWorldY;

    for (TInt retries = 0; retries < 8; retries++) {
      // Don't go the same direction
      TInt direction = Random() & TUint8(3);
      while (direction == mSprite->mDirection) {
        direction = Random() & TUint8(3);
      }

      switch (direction) {
        case 0: // up
          if (sy > 16 && !mPlayfield->IsWall(x + 16, y - 32 - VELOCITY) &&
              !mPlayfield->IsWall(x + 48, y - 32 - VELOCITY)) {
            NewState(WALK_STATE, DIRECTION_UP);
            return ETrue;
          }
          break;
        case 1: // down
          if (sy < (SCREEN_HEIGHT - 16) && !mPlayfield->IsWall(x + 16, y + VELOCITY) &&
              !mPlayfield->IsWall(x + 48, y + VELOCITY)) {
            NewState(WALK_STATE, DIRECTION_DOWN);
            return ETrue;
          }
          break;
        case 2: // left
          if (sx > 16 && !mPlayfield->IsWall(x + 16 - VELOCITY, y + 32) && !mPlayfield->IsWall(x + 16 - VELOCITY, y)) {
            NewState(WALK_STATE, DIRECTION_LEFT);
            return ETrue;
          }
          break;
        case 3: // right
          if (sx < (SCREEN_WIDTH - 16) && !mPlayfield->IsWall(x + 48 + VELOCITY, y + 32) &&
              !mPlayfield->IsWall(x + 48 + VELOCITY, y)) {
            NewState(WALK_STATE, DIRECTION_RIGHT);
            return ETrue;
          }
          break;
        default:
          Panic("GoblinProcess: Invalid mDirection %d\n", mDirection);
          break;
      }
    }

    // after 8 tries, we couldn't find a direction to walk.
#ifdef DEBUGME
    printf("Goblin Can't walk\n");
#endif
    NewState(IDLE_STATE, mSprite->mDirection);
  }

  return ETrue;
}

TBool GOrcProcess::WalkState() {
  if (MaybeHit()) {
    return ETrue;
  }

  BViewPort *vp     = mGameState->GetViewPort();
  TFloat    screenX = mSprite->x - vp->mWorldX,
            screenY = mSprite->y - vp->mWorldY;

#ifdef DEBUGME
  printf("ORC screenX, screenY = %f,%f, x,y = %f,%f\n", screenX, screenY, mSprite->x, mSprite->y);
#endif

  if (--mStateTimer < 0 ||
      mPlayfield->IsWall(mSprite->x + 16 + mSprite->vx, mSprite->y + mSprite->vy) ||      // Left/Bottom Wall
      mPlayfield->IsWall(mSprite->x + 16 + mSprite->vx, mSprite->y - 32 + mSprite->vy) || // Left/Top Wall
      mPlayfield->IsWall(mSprite->x + 48 + mSprite->vx, mSprite->y + mSprite->vy) ||      // Right/Bottom Wall
      mPlayfield->IsWall(mSprite->x + 48 + mSprite->vx, mSprite->y - 32 + mSprite->vy) || // Right/Top Wall
      screenX < 16 || screenX > (SCREEN_WIDTH - 16) || screenY < 16 || screenY > (SCREEN_HEIGHT - 16)
    ) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    NewState(WALK_STATE, mSprite->mDirection);
  }

  return ETrue;
}

TBool GOrcProcess::AttackState() {
  if (MaybeHit()) {
    return ETrue;
  }
  return ETrue;
}

TBool GOrcProcess::HitState() {
  if (mSprite->AnimDone()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    mSprite->cType &= STYPE_PLAYER;
  }

  return ETrue;
}

TBool GOrcProcess::DeathState() {
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

TBool GOrcProcess::RunBefore() {
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

TBool GOrcProcess::RunAfter() {
  return ETrue;
}

