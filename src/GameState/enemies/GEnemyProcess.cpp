#include "GEnemyProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"
#include "GGameState.h"

const TInt HIT_POINTS = 5;  // default hit points for enemy

GEnemyProcess::GEnemyProcess(GGameState *aGameState, TUint16 aSlot, TUint16 aParams)
  : mGameState(aGameState), mPlayfield(aGameState->mGamePlayfield), mParams(aParams) {
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
