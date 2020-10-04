#include <GameState/enemies/GEnemyProcess.h>
#include "GGruntProcess.h"
#include "GPlayer.h"
#include "GGameState.h"
#include "inventory/GItemProcess.h"
#include "Items.h"
#include "common/GSpellOverlayProcess.h"
#include "GameState/enemies/grunts/GEnemyDeathOverlayProcess.h"

#define DEBUGME
#undef DEBUGME

TInt16 GGruntProcess::mCount = 0;

GGruntProcess::GGruntProcess(GGameState *aGameState, TInt aIp, TFloat aX, TFloat aY, TUint16 aSlot, TUint16 aParams, TFloat aVelocity, TUint16 aAttribute)
    : GEnemyProcess(aGameState, aX, aY, aSlot, aAttribute), mIp(aIp), mParams(aParams) {
  mSaveToStream = ETrue;
  mVelocity = aVelocity;
  mStateTimer = 0;
  mState = IDLE_STATE;
  mSprite->SetFlags(SFLAG_RENDER_SHADOW);
  mSprite->w = 32;
  mSprite->h = 16;
  mSprite->cy = 4;
  mRangeX = mRangeY = 8;

  mAttackTimer = 1;
  mTaunt = ETrue;
  mTauntTimer = TauntTime();

  mEnemyDeathOverlayProcess = ENull;
  mSpellOverlayProcess = ENull;
  mCount++;
}

GGruntProcess::~GGruntProcess() {
  mCount--;
}

TBool GGruntProcess::IsWallInDirection(DIRECTION aDirection) {
  return !mSprite->CanWalkInDirection(aDirection);
}

TBool GGruntProcess::CanWalkInDirection(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
  return mSprite->CanWalkInDirection(aDirection, aVx, aVy);
}

/*********************************************************************************
 *********************************************************************************
 *********************************************************************************/

void GGruntProcess::NewState(TUint16 aState, DIRECTION aDirection) {
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

    case TAUNT_STATE:
      SfxTaunt();
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      Taunt(aDirection);
      break;

    case WALK_STATE:
      mStep = 1 - mStep;
      Walk(aDirection);
      break;

    case ATTACK_STATE:
      SfxAttack();
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mAttackTimer = GPlayer::mGameOver ? (1 * FRAMES_PER_SECOND) : ATTACK_TIME;
      Attack(aDirection);
      break;

    case HIT_STATE:
      SfxTakeDamage();
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->cMask &= ~STYPE_EBULLET;
      Hit(aDirection);
      break;

    case SPELL_STATE: {
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->cMask &= ~STYPE_EBULLET;
      auto *p = new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y + 1, 0, 0, 0);
      mSpellOverlayProcess = p;
      mGameState->AddProcess(p);
      Spell(aDirection);
    } break;

    case DEATH_STATE: {
      mSprite->type = STYPE_OBJECT;
      SfxDeath();
      mSaveToStream = EFalse; // Prevent saves while we're animating.
      Death(aDirection);
      auto *p = new GEnemyDeathOverlayProcess(mGameState, this, mSprite->x + 16, mSprite->y + 1);
      mEnemyDeathOverlayProcess = p;
      mGameState->AddProcess(p);
    } break;

    default:
      break;
  }
}

void GGruntProcess::OverlayAnimationComplete() {
  mSpellOverlayProcess = ENull;
  mEnemyDeathOverlayProcess = ENull;
}

TBool GGruntProcess::MaybeHit() {
  if (SpellDamageCheck()) {
    NewState(SPELL_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (BasicDamageCheck()) {
    if (mSprite->mCollided.direction != DIRECTION_UNSPECIFIED) {
      NewState(HIT_STATE, GAnchorSprite::RotateDirection(mSprite->mCollided.direction, 2));
    }
    else {
      NewState(HIT_STATE, mSprite->mDirection);
    }
    return ETrue;
  }

  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->Nudge();
    NewState(IDLE_STATE, mSprite->mDirection);
    return EFalse;
  }

  return EFalse;
}

TBool GGruntProcess::MaybeAttack() {
  TRect myRect, hisRect;
  mSprite->GetRect(myRect);
  GPlayer::mSprite->GetRect(hisRect);

  if (!mGameState->IsGameOver() && !GPlayer::mInvulnerable) {
    if (myRect.y1 <= hisRect.y2 && myRect.y2 >= hisRect.y1) {
      // vertical overlap
      if (myRect.x1 >= hisRect.x2 && myRect.x1 - hisRect.x2 < mRangeX) {
        // to right of player
        if (--mAttackTimer <= 0) {
          NewState(ATTACK_STATE, DIRECTION_LEFT);
        }
        return ETrue;
      }
      if (myRect.x2 <= hisRect.x1 && hisRect.x1 - myRect.x2 < mRangeX) {
        // to left of player
        if (--mAttackTimer <= 0) {
          NewState(ATTACK_STATE, DIRECTION_RIGHT);
        }
        return ETrue;
      }
    } else if (myRect.x1 <= hisRect.x2 && myRect.x2 >= hisRect.x1) {
      // horizontal overlap
      if (myRect.y1 >= hisRect.y2 && myRect.y1 - hisRect.y2 < mRangeY) {
        // below player
        if (--mAttackTimer <= 0) {
          NewState(ATTACK_STATE, DIRECTION_UP);
        }
        return ETrue;
      }
      if (myRect.y2 <= hisRect.y1 && hisRect.y1 - myRect.y2 < mRangeY) {
        // above player
        if (--mAttackTimer <= 0) {
          NewState(ATTACK_STATE, DIRECTION_DOWN);
        }
        return ETrue;
      }
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

TBool GGruntProcess::MaybeTaunt() {
  if (!mTaunt) {
    return EFalse;
  }
  if (--mTauntTimer > 0) {
    return EFalse;
  }
  mTauntTimer = TauntTime();
  NewState(TAUNT_STATE, mSprite->mDirection);
  return ETrue;
}

TBool GGruntProcess::AttackState() {
  // Spells interrupt attack animation, normal attacks don't except for killing blows
  if (SpellDamageCheck()) {
    NewState(SPELL_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (BasicDamageCheck() && mHitPoints <= 0) {
    NewState(HIT_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    mInvulnerable = EFalse;
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GGruntProcess::HitState() {
  if (mSprite->AnimDone()) {
    if (mHitPoints <= 0) {
      NewState(DEATH_STATE, mSprite->mDirection);
      return ETrue;
    }
    mInvulnerable = EFalse;
    mSprite->ClearCType(STYPE_PBULLET);
    NewState(WALK_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GGruntProcess::SpellState() {
  auto *p = mSpellOverlayProcess;
  if (mSprite->AnimDone() && !p) {
    if (mHitPoints <= 0) {
      mSpellOverlayProcess = ENull;
      NewState(DEATH_STATE, mSprite->mDirection);
      return ETrue;
    }

    mInvulnerable = EFalse;
    mSprite->ClearCType(STYPE_PBULLET);
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GGruntProcess::DeathState() {
  auto *p = mEnemyDeathOverlayProcess;
  if (mSprite->AnimDone() && !p) {
    // If we setup a key for the enemy to drop
    if (mParams) {
      printf("drop $%x %d\n", mParams, mParams);
      GItemProcess::SpawnItem(mGameState, mIp, mParams, mSprite->x + 16, mSprite->y + 16);
      gSoundPlayer.TriggerSfx(SFX_PLAYER_QUAFF_HEALTH_POTION_WAV);
      return EFalse;
    }

    // For now, le'ts just drop this and leave potions to crate.
    const TInt32 spawn_threshold = 6;
    TInt32 should_spawn = Random(0, 10);
    if (should_spawn <= spawn_threshold) {
      return EFalse;
    }

    TInt32 item_to_spawn = Random(0, 5);

    switch (item_to_spawn) {
      case 0:
      case 1:
      case 2:
      case 3:
        GItemProcess::SpawnItem(mGameState, -1, ITEM_HEART, mSprite->x + 16, mSprite->y);
        gSoundPlayer.TriggerSfx(SFX_PLAYER_QUAFF_HEALTH_POTION_WAV);
        break;
      case 4:
        GItemProcess::SpawnItem(mGameState, -1, ITEM_BLUE_POTION1, mSprite->x + 16, mSprite->y);
        gSoundPlayer.TriggerSfx(SFX_PLAYER_QUAFF_HEALTH_POTION_WAV);
        break;
//          case 4:
//            GItemProcess::SpawnItem(mGameState, -1, ITEM_BLUE_POTION2, mSprite->x + 16, mSprite->y);
//            break;
      case 5:
        GItemProcess::SpawnItem(mGameState, -1, ITEM_RED_POTION1, mSprite->x + 16, mSprite->y);
        gSoundPlayer.TriggerSfx(SFX_PLAYER_QUAFF_HEALTH_POTION_WAV);
        break;
//          case 3:
//            GItemProcess::SpawnItem(mGameState, -1, ITEM_RED_POTION2, mSprite->x + 16, mSprite->y);
//            break;
      default:
        break;
    }
    return EFalse;
  }

  return ETrue;
}

TBool GGruntProcess::IdleState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeTaunt()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  if (--mStateTimer < 0) {
    // Set distance to walk for WALK_STATE
    for (TInt retries = 0; retries < 8; retries++) {
      DIRECTION direction = GAnchorSprite::RandomDirection();
      TFloat vx = direction == DIRECTION_LEFT ? -mVelocity : direction == DIRECTION_RIGHT ? mVelocity : 0,
             vy = direction == DIRECTION_UP ? -mVelocity : direction == DIRECTION_DOWN ? mVelocity : 0;

      if (CanWalkInDirection(direction, vx, vy)) {
        NewState(WALK_STATE, direction);
        return ETrue;
      }
    }

    // after 8 tries, we couldn't find a direction to walk.
    NewState(TAUNT_STATE, mSprite->mDirection);
  }

  return ETrue;
}

TBool GGruntProcess::TauntState() {
#ifdef DEBUGME
      printf("TauntState\n");
#endif
  if (MaybeHit()) {
#ifdef DEBUGME
      printf("TauntState HIT\n");
#endif
    return ETrue;
  }
  if (mSprite->AnimDone()) {
#ifdef DEBUGME
      printf("TauntState -> idle\n");
#endif
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GGruntProcess::WalkState() {
  if (MaybeAttack()) {
    return ETrue;
  }

  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeTaunt()) {
    return ETrue;
  }

  mAttackTimer = 1;

  if (--mStateTimer < 0) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (!CanWalkInDirection(mSprite->mDirection, mSprite->vx, mSprite->vy)) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->TestAndClearCType(STYPE_OBJECT)) {
    mSprite->Nudge();
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    // done with left/right step, start animation for the other foot
    NewState(WALK_STATE, mSprite->mDirection);
  }

  return ETrue;
}

TBool GGruntProcess::RunBefore() {
  if (mSprite->Clipped() && mHitPoints > 0) {
    mInvulnerable = EFalse;
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }
  switch (mState) {
    case IDLE_STATE:
      return IdleState();
    case TAUNT_STATE:
      return TauntState();
    case WALK_STATE:
      return WalkState();
    case ATTACK_STATE:
      return AttackState();
    case HIT_STATE:
      return HitState();
    case DEATH_STATE:
      return DeathState();
    case SPELL_STATE:
      return SpellState();
    default:
      return ETrue;
  }
}

TBool GGruntProcess::RunAfter() {
  UpdateBlink();
  mSprite->ClearCType(STYPE_PLAYER | STYPE_OBJECT);
  mSprite->mMeter = (TFloat) mHitPoints / (TFloat) mMaxHitPoints;

  return ETrue;
}

void GGruntProcess::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&mIp, sizeof(mIp));
  aStream.Write(&mParams, sizeof(mParams));
  aStream.Write(&mStartX, sizeof(mStartX));
  aStream.Write(&mStartY, sizeof(mStartY));
  aStream.Write(&mState, sizeof(mState));
  aStream.Write(&mDirection, sizeof(mDirection));
  aStream.Write(&mStep, sizeof(mStep));
  aStream.Write(&mAttackTimer, sizeof(mAttackTimer));
  aStream.Write(&mStateTimer, sizeof(mStateTimer));
  aStream.Write(&mVelocity, sizeof(mVelocity));
  mSprite->WriteToStream(aStream);
}

void GGruntProcess::ReadFromStream(BMemoryStream &aStream) {
  aStream.Read(&mIp, sizeof(mIp));
  aStream.Read(&mParams, sizeof(mParams));
  aStream.Read(&mStartX, sizeof(mStartX));
  aStream.Read(&mStartY, sizeof(mStartY));
  aStream.Read(&mState, sizeof(mState));
  aStream.Read(&mDirection, sizeof(mDirection));
  aStream.Read(&mStep, sizeof(mStep));
  aStream.Read(&mAttackTimer, sizeof(mAttackTimer));
  aStream.Read(&mStateTimer, sizeof(mStateTimer));
  aStream.Read(&mVelocity, sizeof(mVelocity));
  mSprite->ReadFromStream(aStream);
  NewState(mState, mSprite->mDirection);
}
