#include "GEnemyProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"
#include "GGameState.h"

const TInt HIT_POINTS = 5;  // default hit points for enemy

GEnemyProcess::GEnemyProcess(GGameState *aGameState, TUint16 aSlot, TUint16 aParams, TFloat aVelocity)
  : mGameState(aGameState), mPlayfield(aGameState->mGamePlayfield), mParams(aParams) {
  mVelocity = aVelocity;
  mStateTimer = 0;
  mHitPoints = HIT_POINTS;
  mState = IDLE_STATE;
  mSprite = new GAnchorSprite(mGameState, 0, aSlot);
  mSprite->mHitPoints = mHitPoints;
  mSprite->type = STYPE_ENEMY;
  mSprite->cMask = STYPE_PLAYER | STYPE_PBULLET;
  mSprite->SetFlags(SFLAG_CHECK);
  mSprite->w = 32;
  mSprite->h = 32;
  mSprite->Name("ENEMY SPRITE");
  mGameState->AddSprite(mSprite);
  mDirection = DIRECTION_DOWN;
  mState = IDLE_STATE;
  mStep = 0;

  mStartX = mStartY = 0;
  mPlayerSprite = GPlayer::mSprite;
  mAttackTimer = 1;
}

GEnemyProcess::~GEnemyProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GEnemyProcess::IsWall(DIRECTION aDirection, TFloat aDx, TFloat aDy) {
  return !mSprite->IsFloor(aDirection, aDx, aDy);
}

TBool GEnemyProcess::CanWalk(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
  return mSprite->CanWalk(aDirection, aVx, aVy);
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GEnemyProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;
  mSprite->mDx = 0;
  mSprite->mDy = 0;

  switch (aState) {

    case IDLE_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      Idle(aDirection);
      break;

    case WALK_STATE:
      mStep = 1 - mStep;
      Walk(aDirection);
      break;

    case ATTACK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      Attack(aDirection);
      break;

    case HIT_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->cMask &= ~STYPE_EBULLET;
      Hit(aDirection);
      break;

    case DEATH_STATE:
      Death(aDirection);
      break;

    default:
      break;
  }
}

TBool GEnemyProcess::MaybeHit() {
  GAnchorSprite *other = mSprite->mCollided;
  if (mSprite->cType & STYPE_PBULLET) {
    if (!mSprite->mInvulnerable) {
      mSprite->Nudge(); // move sprite so it's not on top of player
      mSprite->mInvulnerable = ETrue;
      mSprite->cType &= ~STYPE_PBULLET;
      mSprite->mHitPoints -= other->mHitStrength;
      if (mSprite->mHitPoints <= 0) {
        printf("GEnemy DEATH\n");
        mGameState->AddProcess(new GStatProcess(mSprite->x + 64, mSprite->y, "EXP +%d", mSprite->mLevel));
        NewState(DEATH_STATE, mSprite->mDirection);
        return ETrue;
      } else {
        printf("GEnemy Hit\n");
        mGameState->AddProcess(new GStatProcess(mSprite->x + 64, mSprite->y, "HIT +%d", other->mHitStrength));
      }
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
  }

  if (mSprite->cType & STYPE_PLAYER) {
    mSprite->cType &= ~STYPE_PLAYER;
    mSprite->Nudge();
    return ETrue;
  }

  return EFalse;
}

TBool GEnemyProcess::MaybeAttack() {
  TRect myRect, hisRect;
  mSprite->GetRect(myRect);
  mPlayerSprite->GetRect(hisRect);

  if (!mPlayerSprite->mInvulnerable) {
    if (abs(mPlayerSprite->x - mSprite->x) <= SEEK_X + 16) {
      if (abs(mPlayerSprite->y - mSprite->y) < SEEK_Y) {
        if (--mAttackTimer <= 0) {
          NewState(ATTACK_STATE, mPlayerSprite->x > mSprite->x ? DIRECTION_RIGHT : DIRECTION_LEFT);
          mAttackTimer = FRAMES_PER_SECOND * 3;
        }
        return ETrue;
      } else if (ABS(hisRect.y2 - myRect.y1) < COLLISION_DELTA_Y) {
        if (--mAttackTimer <= 0) {
          NewState(ATTACK_STATE, DIRECTION_UP);
          mAttackTimer = FRAMES_PER_SECOND * 3;
        }
        return ETrue;
      } else if (ABS(myRect.y2 - hisRect.y1) < COLLISION_DELTA_Y) {
        if (--mAttackTimer <= 0) {
          NewState(ATTACK_STATE, DIRECTION_DOWN);
          mAttackTimer = FRAMES_PER_SECOND * 3;
        }
        return ETrue;
      } else {
        mAttackTimer = 1;
      }
    }
  }

  return EFalse;
}

TBool GEnemyProcess::AttackState() {
  if (mSprite->AnimDone()) {
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GEnemyProcess::HitState() {
  if (mSprite->AnimDone()) {
    mSprite->mInvulnerable = EFalse;
    mSprite->cType &= ~STYPE_PBULLET;
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GEnemyProcess::DeathState() {
  if (mSprite->AnimDone()) {
    mSprite->x = mStartX;
    mSprite->y = mStartY;
    mSprite->mInvulnerable = EFalse;
    NewState(IDLE_STATE, mSprite->mDirection);
    mSprite->cType &= ~(STYPE_PLAYER | STYPE_PBULLET);
    mSprite->mHitPoints = mHitPoints;
  }

  return ETrue;
}

TBool GEnemyProcess::IdleState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  if (--mStateTimer < 0) {
    // Set distance to walk for WALK_STATE
    for (TInt retries = 0; retries < 8; retries++) {
      DIRECTION direction = GAnchorSprite::RandomDirection();

      if (CanWalk(direction, mVelocity, mVelocity)) {
        NewState(WALK_STATE, direction);
        return ETrue;
      }
    }

    // after 8 tries, we couldn't find a direction to walk.
    NewState(IDLE_STATE, mSprite->mDirection);
  }

  return ETrue;
}

TBool GEnemyProcess::WalkState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }
  if (--mStateTimer < 0) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (!CanWalk(mSprite->mDirection, mSprite->vx, mSprite->vy)) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->cType & STYPE_PLAYER) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }
  if (mSprite->AnimDone()) {
    NewState(WALK_STATE, mSprite->mDirection);
  }

  return ETrue;
}

TBool GEnemyProcess::RunBefore() {
  if (mSprite->Clipped()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }
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

TBool GEnemyProcess::RunAfter() {
  return ETrue;
}
