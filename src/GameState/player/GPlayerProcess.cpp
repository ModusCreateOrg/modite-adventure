#include "GPlayerProcess.h"
#include "GAnchorSprite.h"
#include "GGamePlayfield.h"
#include "GPlayerAnimations.h"
#include "GStatProcess.h"

#define DEBUGME
#undef DEBUGME

enum {
  IDLE_STATE,
  WALK_STATE,
  ATTACK_STATE,

  HIT_LIGHT_STATE,
  HIT_MEDIUM_STATE,
  HIT_HARD_STATE,

  DEATH_STATE,
};

const TInt16 PLAYER_HITPOINTS = 10;

GPlayerProcess::GPlayerProcess(GGameState *aGameState) : mState(0), mStep(0) {
  mGameState = aGameState;
  mPlayfield = ENull;
  mSprite    = new GAnchorSprite(1, PLAYER_SLOT);
  strcpy(mSprite->mName, "PLAYER");
  mSprite->type       = STYPE_PLAYER;
  mSprite->cMask      = STYPE_ENEMY | STYPE_EBULLET;
  mSprite->x          = mSprite->y = 32;
  mSprite->w          = 32;
  mSprite->h          = 32;
  mGameState->AddSprite(mSprite);
  mSprite->flags |= SFLAG_ANCHOR | SFLAG_SORTY | SFLAG_CHECK;
  mSprite->mHitPoints = PLAYER_HITPOINTS;
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GPlayerProcess::~GPlayerProcess() {
  //
}

void GPlayerProcess::StartLevel(
  GGamePlayfield *aPlayfield, TFloat aX, TFloat aY) {
  mPlayfield = aPlayfield;
  mSprite->x = aX;
  mSprite->y = aY;
}

void GPlayerProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;
  mSprite->mDx        = 0;
  mSprite->mDy        = 0;
  switch (mState) {
    case IDLE_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
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
      }
      break;
    case WALK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mStep = 1 - mStep;
          mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
          mSprite->vy = -VELOCITY;
          break;
        case DIRECTION_DOWN:
          mStep = 1 - mStep;
          mSprite->vy = VELOCITY;
          mSprite->StartAnimation(
            mStep ? walkDownAnimation1 : walkDownAnimation2);
          break;
        case DIRECTION_LEFT:
          mStep = 1 - mStep;
          mSprite->vx = -VELOCITY;
          //          mSprite->mDx = -36;
          mSprite->StartAnimation(
            mStep ? walkLeftAnimation1 : walkLeftAnimation2);
          break;
        case DIRECTION_RIGHT:
          mStep = 1 - mStep;
          mSprite->vx = VELOCITY;
          mSprite->StartAnimation(
            mStep ? walkRightAnimation1 : walkRightAnimation2);
          break;
      }
      break;
    case ATTACK_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mSprite->StartAnimation(swordUpAnimation);
          break;
        case DIRECTION_DOWN:
          mSprite->StartAnimation(swordDownAnimation);
          break;
        case DIRECTION_LEFT:
          mSprite->StartAnimation(swordLeftAnimation);
          break;
        case DIRECTION_RIGHT:
          mSprite->StartAnimation(swordRightAnimation);
          break;
      }
      break;
    case HIT_LIGHT_STATE:
      mSprite->mDx = 0;
      mSprite->mDy = 0;
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mSprite->mInvulnerable = ETrue;
#ifdef DEBUGME
      printf("PLAYER invulnerable\n");
#endif

      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mSprite->StartAnimation(hitLightUpAnimation);
          break;
        case DIRECTION_DOWN:
          mSprite->StartAnimation(hitLightDownAnimation);
          break;
        case DIRECTION_LEFT:
          mSprite->StartAnimation(hitLightLeftAnimation);
          break;
        case DIRECTION_RIGHT:
          mSprite->StartAnimation(hitLightRightAnimation);
          break;
      }
      break;
    case HIT_MEDIUM_STATE:
      mSprite->mDx = 0;
      mSprite->mDy = 0;
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mSprite->mInvulnerable = ETrue;
#ifdef DEBUGME
      printf("PLAYER invulnerable\n");
#endif

      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mSprite->StartAnimation(hitMediumUpAnimation);
          break;
        case DIRECTION_DOWN:
          mSprite->StartAnimation(hitMediumDownAnimation);
          break;
        case DIRECTION_LEFT:
          mSprite->StartAnimation(hitMediumLeftAnimation);
          break;
        case DIRECTION_RIGHT:
          mSprite->StartAnimation(hitLightRightAnimation);
          break;
      }
      break;
    case HIT_HARD_STATE:
      mSprite->mDx = 0;
      mSprite->mDy = 0;
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mSprite->mInvulnerable = ETrue;
#ifdef DEBUGME
      printf("PLAYER invulnerable\n");
#endif

      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mSprite->StartAnimation(hitHardUpAnimation);
          break;
        case DIRECTION_DOWN:
          mSprite->StartAnimation(hitHardDownAnimation);
          break;
        case DIRECTION_LEFT:
          mSprite->StartAnimation(hitHardLeftAnimation);
          break;
        case DIRECTION_RIGHT:
          mSprite->StartAnimation(hitHardRightAnimation);
          break;
      }
      break;
  }
}

TBool GPlayerProcess::MaybeHit() {
  if (mSprite->cType) {
#ifdef DEBUGME
    printf("Player collide cType: %x STYPE_ENEMY: %x STYPE_EBULLET: %x\n",
           mSprite->cType, STYPE_ENEMY, STYPE_EBULLET);
#endif
    TInt state = HIT_LIGHT_STATE;
    if (!mSprite->mInvulnerable && mSprite->cType & STYPE_EBULLET) {
#ifdef DEBUGME
      printf("Player attacked\n");
#endif
      const GAnchorSprite *other = mSprite->mCollided;
      switch (other->mHitStrength) {
        case HIT_LIGHT:
          mSprite->mHitPoints -= 1;
          mSprite->mInvulnerable = ETrue;
          mGameState->AddProcess(
            new GStatProcess(mSprite->x - 32, mSprite->y - 63, "HIT +1"));
          break;
        case HIT_MEDIUM:
          mSprite->mHitPoints -= 2;
          mSprite->mInvulnerable = ETrue;
          state = HIT_MEDIUM_STATE;
          mGameState->AddProcess(
            new GStatProcess(mSprite->x - 32, mSprite->y - 63, "HIT +2"));
          break;
        case HIT_HARD:
          mSprite->mHitPoints -= 3;
          mSprite->mInvulnerable = ETrue;
          state = HIT_HARD_STATE;
          mGameState->AddProcess(
            new GStatProcess(mSprite->x - 32, mSprite->y - 63, "HIT +3"));
          break;
      }

      if (mSprite->mHitPoints <= 0) {
        // GAME OVER!
#ifdef DEBUGME
        printf("Player dead\n");
#endif
        mSprite->mHitPoints = PLAYER_HITPOINTS;
      }
    }
    mSprite->cType = 0;

    // bounce player off enemy
    const GAnchorSprite *enemy = mSprite->mCollided;
    if (mSprite->vx) {
      if (mSprite->x < enemy->x) {
        mSprite->x = enemy->x - 34;
      } else {
        mSprite->x = enemy->x + 34;
      }
    }
    else if (mSprite->vy) {
      if (mSprite->y < enemy->y) {
        mSprite->y = enemy->y - 6;
      } else {
        mSprite->y = enemy->y + 6;
      }
    }
    else if (abs(enemy->y - mSprite->y) > COLLISION_DELTA_Y) {
      if (enemy->x > mSprite->x) {
        mSprite->x = enemy->x - 36;
      }
      else {
        mSprite->x = enemy->x + 36;
      }
    }
    else {
      mSprite->y = enemy->y;
    }

    mSprite->mInvulnerable = ETrue;
    mSprite->cType = 0;
    NewState(state, mSprite->mDirection);
    return ETrue;
  }
  mSprite->cType = 0;
  return EFalse;
}

TBool GPlayerProcess::MaybeSword() {
  if (!gControls.WasPressed(BUTTONA)) {
    return EFalse;
  }
  NewState(ATTACK_STATE, mSprite->mDirection);
  return ETrue;
}

TBool GPlayerProcess::MaybeWalk() {
  // instead of walking, we might have to attack if A is pressed
  if (MaybeSword()) {
    return ETrue;
  }
  if (gControls.IsPressed(JOYLEFT)) {
    if (mSprite->x - VELOCITY < 0 ||
        mPlayfield->IsWall(mSprite->x + 26 - VELOCITY, mSprite->y - 2) ||
        mPlayfield->IsWall(mSprite->x + 26 - VELOCITY, mSprite->y - 8)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_LEFT) {
      NewState(WALK_STATE, DIRECTION_LEFT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYRIGHT)) {
    if (mPlayfield->IsWall(mSprite->x + 38 + VELOCITY, mSprite->y - 2) ||
        mPlayfield->IsWall(mSprite->x + 38 + VELOCITY, mSprite->y - 8)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_RIGHT) {
      NewState(WALK_STATE, DIRECTION_RIGHT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYUP)) {
    if (mSprite->y - VELOCITY < 0 ||
        mPlayfield->IsWall(mSprite->x + 26, mSprite->y - 8 - VELOCITY) ||
        mPlayfield->IsWall(mSprite->x + 38, mSprite->y - 8 - VELOCITY)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_UP) {
      NewState(WALK_STATE, DIRECTION_UP);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYDOWN)) {
    if (mPlayfield->IsWall(mSprite->x + 26, mSprite->y - 2 + VELOCITY) ||
        mPlayfield->IsWall(mSprite->x + 38, mSprite->y - 2 + VELOCITY)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_DOWN) {
      NewState(WALK_STATE, DIRECTION_DOWN);
    }
    return ETrue;
  }

  return EFalse;
}

TBool GPlayerProcess::IdleState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (MaybeSword()) {
    return ETrue;
  }
  MaybeWalk();
  return ETrue;
}

TBool GPlayerProcess::WalkState() {
  if (MaybeSword()) {
    return ETrue;
  }

  // maybe change direction!
  if (!MaybeWalk()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  // collision?
  if (MaybeHit()) {
//    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  // can player keep walking?
  switch (mSprite->mDirection) {
    case DIRECTION_LEFT:
      if (mPlayfield->IsWall(
        mSprite->x + 26 + mSprite->vx, mSprite->y - 2 + mSprite->vy) ||
          mPlayfield->IsWall(
            mSprite->x + 26 + mSprite->vx, mSprite->y - 8 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      if (mPlayfield->IsWall(
        mSprite->x + 38 + mSprite->vx, mSprite->y - 2 + mSprite->vy) ||
          mPlayfield->IsWall(
            mSprite->x + 38 + mSprite->vx, mSprite->y - 8 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_UP:
      if (mPlayfield->IsWall(mSprite->x + 26, mSprite->y - 8 + mSprite->vy) ||
          mPlayfield->IsWall(mSprite->x + 38, mSprite->y - 8 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_DOWN:
      if (mPlayfield->IsWall(mSprite->x + 26, mSprite->y - 2 + mSprite->vy) ||
          mPlayfield->IsWall(mSprite->x + 38, mSprite->y - 2 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
  }

  if (mSprite->AnimDone()) {
    NewState(WALK_STATE, mSprite->mDirection);
  }

  return ETrue;
}

TBool GPlayerProcess::SwordState() {
  if (mSprite->AnimDone()) {
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GPlayerProcess::HitState() {
  if (mSprite->AnimDone()) {
    mSprite->cType = 0;
#ifdef DEBUGME
    printf("VULNERABLE\n");
#endif
    mSprite->mInvulnerable = EFalse;
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GPlayerProcess::RunBefore() {
  switch (mState) {
    case IDLE_STATE:
      return IdleState();
    case WALK_STATE:
      return WalkState();
    case ATTACK_STATE:
      return SwordState();
    case HIT_LIGHT_STATE:
    case HIT_MEDIUM_STATE:
    case HIT_HARD_STATE:
      return HitState();
    default:
      return ETrue;
  }
}

TBool GPlayerProcess::RunAfter() {
  // position viewport to follow player
  const TFloat xx = mSprite->x - gViewPort->mWorldX,
               yy = mSprite->y - 16 - gViewPort->mWorldY,
               dx = gViewPort->mRect.Width(), dy = gViewPort->mRect.Height();

  // Don't go less than 0 or more than map width/height
  if (xx > dx) {
    gViewPort->mWorldX = MIN(mGameState->MapWidth(), gViewPort->mWorldX + dx);
  } else if (xx < 0) {
    gViewPort->mWorldX = MAX(0, gViewPort->mWorldX - dx);
  } else if (yy > dy) {
    gViewPort->mWorldY = MIN(mGameState->MapHeight(), gViewPort->mWorldY + dy);
  } else if (yy < 0) {
    gViewPort->mWorldY = MAX(0, gViewPort->mWorldY - dy);
  }

  return ETrue;
}
