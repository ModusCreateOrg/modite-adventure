#include "GEnemyProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"
#include "GGameState.h"

const TInt HIT_POINTS = 5; // default hit points for enemy

const TInt16 DEATH_SPEED = 4;

// ANIMATIONS


static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(ENEMY_DEATH_SLOT),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 0),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 1),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 2),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 3),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 4),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 5),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 6),
  ASTEP(DEATH_SPEED, IMG_ENEMY_DEATH + 7),
  AEND,
};

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

  mDeathSprite = ENull;
}

GEnemyProcess::~GEnemyProcess() {
  if (mDeathSprite) {
    mDeathSprite->Remove();
    delete mDeathSprite;
    mDeathSprite = ENull;
  }
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
      mAttackTimer = ATTACK_TIME;
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
//      Death(aDirection);
      break;

    default:
      break;
  }
}

TBool GEnemyProcess::MaybeHit() {
  GAnchorSprite *other = mSprite->mCollided;
  if (mSprite->TestCType(STYPE_PBULLET)) {
    mSprite->ClearCType(STYPE_PBULLET);
    if (!mSprite->mInvulnerable) {
      mSprite->Nudge(); // move sprite so it's not on top of player
      mSprite->mInvulnerable = ETrue;
      //      mSprite->ClearCType(STYPE_PBULLET);
      mSprite->mHitPoints -= other->mHitStrength;
      if (mSprite->mHitPoints <= 0) {
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
//        GPlayer::AddExperience(mSprite->mLevel);
//        return ETrue;
      }
      else {
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "HIT +%d", other->mHitStrength));
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

  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->Nudge();
    return ETrue;
  }

  return EFalse;
}

static const TFloat DX = 20,
                    DY = 34;

TBool GEnemyProcess::MaybeAttack() {
  TRect myRect, hisRect;
  mSprite->GetRect(myRect);
  mPlayerSprite->GetRect(hisRect);

  if (!mPlayerSprite->mInvulnerable) {
    if (myRect.x1 >= hisRect.x2) {
      // to right of player
      if (ABS(hisRect.x2 - myRect.x1) > DX) {
        mAttackTimer = 1;
        return EFalse;
      }
      if (ABS(mPlayerSprite->y - mSprite->y) > DY) {
        mAttackTimer = 1;
        return EFalse;
      }
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_LEFT);
      }
      return ETrue;
    }
    else if (myRect.x2 <= hisRect.x1) {
      // to left of player
      if (ABS(hisRect.x1 - myRect.x2) > DX) {
        mAttackTimer = 1;
        return EFalse;
      }
      if (ABS(mPlayerSprite->y - mSprite->y) > DY) {
        mAttackTimer = 1;
        return EFalse;
      }
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_RIGHT);
      }
      return ETrue;
    }

    // player and enemy overlap in x direction
    if (myRect.y1 >= hisRect.y2) {
      // enemy below player
      if (ABS(mPlayerSprite->y - mSprite->y) > DY) {
        // too far away
        mAttackTimer = 1;
        return EFalse;
      }
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_UP);
      }
      return ETrue;
    }
    else if (myRect.y2 <= hisRect.y1) {
      // enemy above player
      if (ABS(mPlayerSprite->y - mSprite->y) > DY) {
        // too far away
        mAttackTimer = 1;
        return EFalse;
      }
      if (--mAttackTimer <= 0) {
        NewState(ATTACK_STATE, DIRECTION_DOWN);
      }
      return ETrue;
    }

    // player not near us
    mAttackTimer = 1;
    return EFalse;
  }
  else {
    //    printf("Player Invulnerable\n");
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
  if (mSprite->mHitPoints <= 0) {
      NewState(DEATH_STATE, mSprite->mDirection);
      return ETrue;
    }
    mSprite->mInvulnerable = EFalse;
    mSprite->ClearCType(STYPE_PBULLET);
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GEnemyProcess::DeathState() {
  if (mSprite->AnimDone()) {
    if (mDeathSprite) {
      if (mDeathSprite->AnimDone()) {
        GPlayer::AddExperience(mSprite->mLevel);
        return EFalse;
      }
      else {
        return ETrue;
      }
    }
    mDeathSprite = new GAnchorSprite(mGameState, -10, ENEMY_DEATH_SLOT);
    mDeathSprite->x = mSprite->x + 16;
    mDeathSprite->y = mSprite->y;
    mDeathSprite->Name("ENEMY DEATH SPRITE");
    mDeathSprite->ClearFlags(SFLAG_CHECK);
    mGameState->AddSprite(mDeathSprite);
    mDeathSprite->StartAnimation(deathAnimation);
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
      TFloat vx = direction == DIRECTION_LEFT ? -mVelocity : mVelocity,
             vy = direction == DIRECTION_UP ? -mVelocity : mVelocity;

      if (CanWalk(direction, vx, vy)) {
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

  mAttackTimer = 1;

  if (--mStateTimer < 0) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (!CanWalk(mSprite->mDirection, mSprite->vx, mSprite->vy)) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    // done with left/right step, start animation for the other foot
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
