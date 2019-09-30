#include "GEnemyProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"
#include "GGameState.h"

GEnemyProcess::GEnemyProcess(
    GGameState *aGameState, GGamePlayfield *aGamePlayfield, TUint16 aSlot)
    : mGameState(aGameState), mPlayfield(aGamePlayfield) {
  mState = IDLE_STATE;
  mSprite = new GAnchorSprite(0, aSlot);
  mSprite->type = STYPE_ENEMY;
  mSprite->cMask = STYPE_PLAYER | STYPE_PBULLET;
  mSprite->flags |= SFLAG_CHECK;
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

TBool GEnemyProcess::IsWall(TInt aDirection) {
  const TInt leftX = mSprite->x + mSprite->w / 2,
             rightX = mSprite->mRect.x2 -
                      mSprite->w / 2, //  - leftX + mSprite->w - 1,
      top = mSprite->y - mSprite->h, bottom = mSprite->y;

  switch (aDirection) {
    case DIRECTION_UP:
      if (mPlayfield->IsWall(leftX, top - 1)) {
        return ETrue;
      }
      if (mPlayfield->IsWall(rightX, top - 1)) {
        return ETrue;
      }
      break;
    case DIRECTION_DOWN:
      if (mPlayfield->IsWall(leftX, bottom + 1)) {
        return ETrue;
      }
      if (mPlayfield->IsWall(rightX, bottom + 1)) {
        return ETrue;
      }
      break;
    case DIRECTION_LEFT:
      if (mPlayfield->IsWall(leftX - 8, top)) {
        return ETrue;
      }
      if (mPlayfield->IsWall(leftX - 8, bottom)) {
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      if (mPlayfield->IsWall(rightX + 8, top)) {
        return ETrue;
      }
      if (mPlayfield->IsWall(rightX + 8, bottom)) {
        return ETrue;
      }
      break;
    default:
      Panic("GEnemyProcess::IsWall invalid direction %d\n", aDirection);
      break;
  }

  return EFalse;
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

TBool GEnemyProcess::MaybeHit() {
  GAnchorSprite *other = mSprite->mCollided;
  if (mSprite->cType & STYPE_PBULLET) {
    if (!mSprite->mInvulnerable) {
      mSprite->Nudge(); // move sprite so it's not on top of player
      mSprite->mInvulnerable = ETrue;
      mSprite->cType &= ~STYPE_PBULLET;
      mSprite->mHitPoints -= other->mHitStrength;
      if (mSprite->mHitPoints <= 0) {
        mGameState->AddProcess(new GStatProcess(mSprite->x, mSprite->y - 32, "EXP +%d", mSprite->mLevel));
        NewState(DEATH_STATE, mSprite->mDirection);
        return ETrue;
      } else {
        mGameState->AddProcess(
          new GStatProcess(mSprite->x, mSprite->y - 32, "HIT +%d", mSprite->mCollided->mHitStrength));
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
  if (!mPlayerSprite->mInvulnerable) {
    if (abs(mPlayerSprite->y - mSprite->y) < SEEK_Y) {
      if (abs(mPlayerSprite->x - mSprite->x) <= SEEK_X + 16) {
        if (--mAttackTimer <= 0) {
          NewState(ATTACK_STATE,
              mPlayerSprite->x > mSprite->x ? DIRECTION_RIGHT : DIRECTION_LEFT);
          mAttackTimer = FRAMES_PER_SECOND * 3;
          return ETrue;
        }
      }
    }
    else {
      mAttackTimer = 1;
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

TBool GEnemyProcess::RunBefore() {
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
