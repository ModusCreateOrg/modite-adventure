#include "GPlayerProcess.h"
#include "GPlayer.h"
#include "GPlayerSprite.h"
#include "GGamePlayfield.h"
#include "GPlayerAnimations.h"
#include "GStatProcess.h"
#include "GResources.h"
#include "GPlayerBulletProcess.h"
#include "GBossProcess.h"
#include "../../common/GSpellOverlayProcess.h"
#include "GBossProcess.h"
#include "../../common/GSpellOverlayProcess.h"

#define DEBUGME
#undef DEBUGME

const TFloat SPELL_DISTANCE = 200.0;

const TUint16 IDLE_STATE = 0;
const TUint16 WALK_STATE = 1;
const TUint16 SWORD_CHARGE_STATE = 2;
const TUint16 SWORD_ATTACK_STATE = 3;
const TUint16 FALL_STATE = 4;
const TUint16 HIT_LIGHT_STATE = 5;
const TUint16 HIT_MEDIUM_STATE = 6;
const TUint16 HIT_HARD_STATE = 7;
const TUint16 QUAFF_STATE = 8;
const TUint16 SPELL_STATE = 9;

const TInt16 BLINK_TIME = FRAMES_PER_SECOND;

DIRECTION GPlayerProcess::mLastDirection = DIRECTION_DOWN;
TFloat GPlayerProcess::mRespawnAt[2] = { '\0', '\0' };

GPlayerProcess::GPlayerProcess(GGameState *aGameState) : GProcess(ATTR_PLAYER_IN1), mGameState(aGameState) {
  mState = IDLE_STATE;
  mStep = 0;
  mStepFrame = 0;
  mBlinkTimer = 0;
  mPlayfield = ENull;
  GPlayer::mInvulnerable = EFalse;
  GPlayer::mSprite = mSprite = ENull;

  // initialize player sprite
  GPlayer::mSprite = mSprite = new GPlayerSprite(mGameState);
  mGameState->AddSprite(mSprite);

  mSprite2 = ENull;

  NewState(IDLE_STATE, mLastDirection);
}

GPlayerProcess::~GPlayerProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    GPlayer::mSprite = mSprite = ENull;
    GPlayer::mProcess = ENull;
  }
}

void GPlayerProcess::StartLevel(GGamePlayfield *aPlayfield, TFloat aX, TFloat aY, TInt16 aExitingDungeon, TInt16 aExitingLevel) {
  mPlayfield = aPlayfield;

  if (aExitingDungeon == OVERWORLD_DUNGEON) {
    mSprite->x = aX;
    mSprite->y = aY;
    mRespawnAt[0] = aX;
    mRespawnAt[1] = aY;

    if (aExitingLevel == 0) {
      printf("aExitingLevel == 0\n");
      return;
    }


    TInt objectCount = mPlayfield->mObjectCount;
    BObjectProgram *program = mPlayfield->mObjectProgram;



    for (TInt ip = 0; ip < objectCount; ip++) {
      const TUint16 op = program[ip].mCode & TUint32(0xffff);

      if (op != ATTR_OW_LEVEL_ENTRANCE) {
        continue;
      }

      const TUint16 params = program[ip].mCode >> TUint32(16),
            row = program[ip].mRow,
            col = program[ip].mCol;

      const TInt dungeon = params >> 8;
//      printf("OVERWORLD ENTRANCE row,col = %d,%d params = %d/%x %d\n", row, col, params, params, dungeon);
//      printf("GetMapHeight() = %i, GetMapWidth() = %i\n", mPlayfield->GetMapHeight(), mPlayfield->GetMapWidth());

      if (aExitingLevel == params) {
        auto xx = TFloat(col * 32), yy = TFloat(row * 32);

        if (row == 0) {
          // Heading down
          mSprite->x = xx - 16;
          mSprite->y = yy + 32;
        }
        else if (row == mPlayfield->GetMapHeight() - 1) {
          // Heading Up
          mSprite->x = xx - 16;
          mSprite->y = yy + 24;
        }
        else if (col == 0) {
          // Heading Right
          mSprite->x = xx - 16;
          mSprite->y = yy + 32;
        }
        else if (col == mPlayfield->GetMapWidth() - 1) {
          // Heading Left
          mSprite->x = xx - 24;
          mSprite->y = yy + 32;
        }

        mRespawnAt[0] = mSprite->x;
        mRespawnAt[1] = mSprite->y;
        return;
      }
    }



  }
  else {
    // player is exiting a dungeon to the overworld, so we need to scan the object program to find our starting position
    TInt objectCount = mPlayfield->mObjectCount;
    BObjectProgram *program = mPlayfield->mObjectProgram;

    for (TInt ip = 0; ip < objectCount; ip++) {
      const TUint16 op = program[ip].mCode & TUint32(0xffff);

      if (op != ATTR_STONE_STAIRS_DOWN) {
        continue;
      }

      const TUint16 params = program[ip].mCode >> TUint32(16),
          row = program[ip].mRow,
          col = program[ip].mCol;

      const TInt dungeon = params >> 8;
      printf("DUNGEON ENTRANCE row,col = %d,%d params = %d/%x %d\n", row, col, params, params, dungeon);
      if (aExitingDungeon == dungeon) {
        auto xx = TFloat(col * 32), yy = TFloat(row * 32);
        mSprite->x = xx - 16;
        mSprite->y = yy + 64;
        mRespawnAt[0] = mSprite->x;
        mRespawnAt[1] = mSprite->y;
        return;
      }
    }
    Panic("Could not find dungeon entrance %d\n", aExitingDungeon);
  }

}

TBool GPlayerProcess::IsLedge() {
  TRect r;
  mSprite->GetRect(r);

  return mPlayfield->IsLedge(r.x1, r.y2) ||
         mPlayfield->IsLedge(r.x2, r.y2);
}

TBool GPlayerProcess::CanWalk(TFloat aVx, TFloat aVy) {
  return mSprite->CanWalk(aVx, aVy);
}

void GPlayerProcess::StartKnockback() {
  GCollidedData other = mSprite->mCollided;
  if (other.flags & SFLAG_KNOCKBACK) {
    TFloat newVx, newVy;

    newVx = PLAYER_VELOCITY * SIN(other.collisionAngle);
    newVy = PLAYER_VELOCITY * COS(other.collisionAngle);

    // if other sprite is moving towards player, add its momentum to player knockback
    if (newVx > 0 ^ other.vx < 0) {
      newVx += other.vx;
    }
    if (newVy > 0 ^ other.vy < 0) {
      newVy += other.vy;
    }

    if (CanWalk(newVx, 0)) {
      mSprite->vx = newVx;
    }
    if (CanWalk(0, newVy)) {
      mSprite->vy = newVy;
    }
  }
}

void GPlayerProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;
  mLastDirection = aDirection;

  mSprite->mDx = 0;
  mSprite->mDy = -4;
  switch (mState) {

    case WALK_STATE:
      if (mStepFrame > 0) {
        if ((aDirection == DIRECTION_UP && mSprite->vy < 0) ||
            (aDirection == DIRECTION_DOWN && mSprite->vy > 0) ||
            (aDirection == DIRECTION_LEFT && mSprite->vx < 0) ||
            (aDirection == DIRECTION_RIGHT && mSprite->vx > 0)) {
          mStep = (mStep + 1) % 4;
        } else {
          // invert walk animation order if walking backwards
          mStep = (mStep + 3) % 4;
        }
        switch (mStep) {
          case 0:
          default:
            mSprite->StartAnimationInDirection(walkAnimations1, aDirection);
            break;
          case 1:
            mSprite->StartAnimationInDirection(walkAnimations2, aDirection);
            break;
          case 2:
            mSprite->StartAnimationInDirection(walkAnimations3, aDirection);
            break;
          case 3:
            mSprite->StartAnimationInDirection(walkAnimations4, aDirection);
            break;
        }
        break;
      }
    case IDLE_STATE:
      mStep = 0;
      mSprite->StartAnimationInDirection(idleAnimations, aDirection);
      break;
    case SWORD_CHARGE_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mSprite->StartAnimationInDirection(swordChargeAnimations, aDirection);
      break;
    case SWORD_ATTACK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;

      gSoundPlayer.SfxPlayerSlash();
      mSprite->StartAnimationInDirection(GPlayer::mEquipped.mGloves ? swordAnimationsWithGloves : swordAnimationsNoGloves, aDirection);
      break;

    case FALL_STATE:
      mStep = 0;
      mStepFrame = 0;
      mSprite->vx = 0;
      mSprite->vy = PLAYER_VELOCITY;
      mSprite->mDy = -4;
      mSprite->StartAnimation(fallAnimation);
      mSprite->mDirection = DIRECTION_DOWN;
      break;

    case SPELL_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;

      GPlayer::mInvulnerable = ETrue;
      mSprite->StartAnimation(spell1Animation);
      mSprite->mDirection = DIRECTION_DOWN;

      break;
  }
}

/*
  ____ _   _    _    _   _  ____ _____   ____ _____  _  _____ _____
 / ___| | | |  / \  | \ | |/ ___| ____| / ___|_   _|/ \|_   _| ____|
| |   | |_| | / _ \ |  \| | |  _|  _|   \___ \ | | / _ \ | | |  _|
| |___|  _  |/ ___ \| |\  | |_| | |___   ___) || |/ ___ \| | | |___
 \____|_| |_/_/   \_\_| \_|\____|_____| |____/ |_/_/   \_\_| |_____|
 */

TBool GPlayerProcess::MaybeSpell() {
  if (GPlayer::mGameOver) {
    return EFalse;
  }
  if (gControls.WasPressed(CONTROL_SPELL)) {
    if (GPlayer::mManaPotion >= 25 && GPlayer::mEquipped.mSpellBookElement) {
      GPlayer::mManaPotion -= 25;
      NewState(SPELL_STATE, DIRECTION_DOWN);
    }
    return ETrue;
  }
  return EFalse;
}

TBool GPlayerProcess::MaybeHit() {
  if (mSprite->TestCType(STYPE_EBULLET | STYPE_ENEMY | STYPE_OBJECT)) {
    mSprite->Nudge();
  }

  if (GPlayer::mInvulnerable) {
    mSprite->cType = 0;
    return EFalse;
  }

  GCollidedData other = mSprite->mCollided;
  TInt hitAmount = 0;

  if (mSprite->TestAndClearCType(STYPE_EBULLET)) {
    hitAmount = other.attackStrength;

    if (hitAmount <= GPlayer::mMaxHitPoints * 0.15) {
      if (other.direction == DIRECTION_UNSPECIFIED) {
        mSprite->StartAnimationInDirection(hitLightAnimations, mSprite->mDirection);
      }
      else {
        mSprite->StartAnimationInDirection(hitLightAnimations, GAnchorSprite::RotateDirection(other.direction, 2));
      }
    }
    else if (hitAmount <= GPlayer::mMaxHitPoints * 0.30) {
      if (other.direction == DIRECTION_UNSPECIFIED) {
        mSprite->StartAnimationInDirection(hitMediumAnimations, mSprite->mDirection);
      }
      else {
        mSprite->StartAnimationInDirection(hitMediumAnimations, GAnchorSprite::RotateDirection(other.direction, 2));
      }
    }
    else {
      if (other.direction == DIRECTION_UNSPECIFIED) {
        mSprite->StartAnimationInDirection(hitHardAnimations, mSprite->mDirection);
      }
      else {
        mSprite->StartAnimationInDirection(hitHardAnimations, GAnchorSprite::RotateDirection(other.direction, 2));
      }
    }
  }

  if (mSprite->TestAndClearCType(STYPE_ENEMY)) {
    // contact damage independent of enemy attack strength
    hitAmount = BASE_STRENGTH;
    mSprite->StartAnimationInDirection(hitMediumAnimations, GAnchorSprite::RotateDirection(other.direction, 2));
  }

  if (hitAmount) {
    TInt state = HIT_LIGHT_STATE;
    // Random +/- 20% variation
    hitAmount = (hitAmount * Random(80, 120)) / 100;


    if (GPlayer::mEquipped.mAmuletElement && other.element) {
      hitAmount *= AMULET_MATRIX[GPlayer::mEquipped.mAmuletElement - 1][other.element - 1];
    }

    GPlayer::mHitPoints -= hitAmount;
    GPlayer::mInvulnerable = ETrue;
    mGameState->AddProcess(new GStatProcess(STAT_PLAYER_HIT, mSprite->Center(), "%d", hitAmount));

    StartBlink(BLINK_TIME);

    gSoundPlayer.TriggerSfx(SFX_PLAYER_TAKE_DAMAGE_WAV);

    mState = state;

    if (GPlayer::mHitPoints <= 0) {
      // PLAYER DEAD
      GPlayer::mHitPoints = 0;
      //      printf("Player dead\n");
      // TO RESUME:
      //      GPlayer::mHitPoints = GPlayer::mMaxHitPoints;
      if (!GPlayer::mGameOver) {
        mGameState->GameOver();
        // don't collide with enemy, enemy attacks, and environment
        mSprite->ClearCMask(STYPE_ENEMY | STYPE_EBULLET | STYPE_OBJECT);
      }
    }

    StartKnockback();

    mSprite->cType = 0;
    return ETrue;
  }

  return EFalse;
}

TBool GPlayerProcess::MaybeSword() {
  if (GPlayer::mGameOver) {
    return ETrue;
  }
  if (gControls.WasPressed(CONTROL_FIRE)) {
    mStep = mStepFrame = 0;
    mSprite->mSwordCharge = -TFloat(CHARGE_START_DELAY) / CHARGE_DURATION;
    NewState(SWORD_CHARGE_STATE, mSprite->mDirection);
    return ETrue;
  }

  return EFalse;
}

TBool GPlayerProcess::MaybeFall() {
  if (IsLedge() && gControls.IsPressed(CONTROL_JOYDOWN)) {
    NewState(FALL_STATE, DIRECTION_DOWN);
    return ETrue;
  }
  return EFalse;
}

DIRECTION GPlayerProcess::MaybeMove(TFloat aSpeed) {
  TFloat newVx = 0.0, newVy = 0.0;
  DIRECTION newDirection = mSprite->mDirection;
  if (gControls.IsPressed(CONTROL_JOYUP)) {
    newVy = -aSpeed;
    newDirection = DIRECTION_UP;
  }
  else if (gControls.IsPressed(CONTROL_JOYDOWN)) {
    newVy = aSpeed;
    newDirection = DIRECTION_DOWN;
  }
  if (gControls.IsPressed(CONTROL_JOYLEFT)) {
    if (newVy) {
      newVx = -aSpeed * M_SQRT2 / 2;
      newVy = newVy * M_SQRT2 / 2;
    }
    else {
      newVx = -aSpeed;
    }
    if (newVy == 0 || newVx != 0) {
      newDirection = DIRECTION_LEFT;
    }
  }
  else if (gControls.IsPressed(CONTROL_JOYRIGHT)) {
    if (newVy) {
      newVx = aSpeed * M_SQRT2 / 2;
      newVy = newVy * M_SQRT2 / 2;
    }
    else {
      newVx = aSpeed;
    }
    if (newVy == 0 || newVx != 0) {
      newDirection = DIRECTION_RIGHT;
    }
  }

  GFloatRect r;
  mSprite->GetFloatRect(r);
  if (newVx && !CanWalk(newVx, 0)) {
    if (newVy == 0) {
      if (newVx < 0 && mSprite->IsFloorTile(r.x1 + newVx, r.y1) != mSprite->IsFloorTile(r.x1 + newVx, r.y2)) {
        newVx = newVx * M_SQRT2 / 2;
        newVy = mSprite->IsFloorTile(r.x1 + newVx, r.y1) ? newVx : -newVx;
      }
      if (newVx > 0 && mSprite->IsFloorTile(r.x2 + newVx, r.y1) != mSprite->IsFloorTile(r.x2 + newVx, r.y2)) {
        newVx = newVx * M_SQRT2 / 2;
        newVy = mSprite->IsFloorTile(r.x2 + newVx, r.y1) ? -newVx : newVx;
      }
    }
  }
  if (newVy && !CanWalk(0, newVy)) {
    if (newDirection == DIRECTION_UP || newDirection == DIRECTION_DOWN) {
      if (newVy < 0 && mSprite->IsFloorTile(r.x1, r.y1 + newVy) != mSprite->IsFloorTile(r.x2, r.y1 + newVy)) {
        newVy = newVy * M_SQRT2 / 2;
        newVx = mSprite->IsFloorTile(r.x1, r.y1 + newVy) ? newVy : -newVy;
      }
      if (newVy > 0 && mSprite->IsFloorTile(r.x1, r.y2 + newVy) != mSprite->IsFloorTile(r.x2, r.y2 + newVy)) {
        newVy = newVy * M_SQRT2 / 2;
        newVx = mSprite->IsFloorTile(r.x1, r.y2 + newVy) ? -newVy : newVy;
      }
    }
  }

  if (!CanWalk(newVx, newVy)) {
    if (CanWalk(newVx, 0)) {
      TFloat savedVx = newVx;
      while (newVx && !CanWalk(newVx, newVy)) {
        newVx = newVx > 0 ? MAX(0, newVx - 1) : MIN(0, newVx + 1);
      }
      while (newVy && !CanWalk(newVx, newVy)) {
        newVx = savedVx;
        newVy = newVy > 0 ? MAX(0, newVy - 1) : MIN(0, newVy + 1);
      }
    } else if (CanWalk(0, newVy)) {
      TFloat savedVy = newVy;
      while (newVy && !CanWalk(newVx, newVy)) {
        newVy = newVy > 0 ? MAX(0, newVy - 1) : MIN(0, newVy + 1);
      }
      while (newVx && !CanWalk(newVx, newVy)) {
        newVy = savedVy;
        newVx = newVx > 0 ? MAX(0, newVx - 1) : MIN(0, newVx + 1);
      }
    } else {
      newVx = newVy = 0;
    }
  }

  if (mSprite->TestAndClearCType(STYPE_OBJECT)) {
    newVx = newVy = 0;
  }

  mSprite->vy = newVy;
  mSprite->vx = newVx;

  if (GPlayer::mTargeted && !gControls.IsPressed(CONTROL_RUN)) {
    TPoint myCenter = mSprite->Center(), otherCenter = GPlayer::mTargeted->Center();
    newDirection = GAnchorSprite::VectorToDirection(otherCenter.x - myCenter.x, otherCenter.y - myCenter.y);
  }

  return newDirection;
}

TBool GPlayerProcess::MaybeWalk() {
  if (GPlayer::mGameOver) {
    return ETrue;
  }

  // Disabled by JG 5/5/2020 -- we're going to use the out of the box shadows and can't use this state b/c of this.
//  if (MaybeFall()) {
//    return EFalse;
//  }

  TFloat speed = PLAYER_VELOCITY;
  if (gControls.IsPressed(CONTROL_RUN)) {
    if (GPlayer::mEquipped.mBoots) {
      speed *= 2.0;
    } else {
      speed *= 1.4;
    }
  }

  DIRECTION newDirection = MaybeMove(speed);

  if (mSprite->vx == 0 && mSprite->vy == 0) {
    return EFalse;
  }
  else if (mState != WALK_STATE || mSprite->mDirection != newDirection) {
    mStep = 0;
    NewState(WALK_STATE, newDirection);
  }
  return ETrue;
}

/*____ _____  _  _____ _____ ____
/ ___|_   _|/ \|_   _| ____/ ___|
\___ \ | | / _ \ | | |  _| \___ \
 ___) || |/ ___ \| | | |___ ___) |
|____/ |_/_/   \_\_| |_____|____/
*/
TBool GPlayerProcess::IdleState() {
  // collision?
  mStepFrame = 0;
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeSpell()) {
    return ETrue;
  }

  if (MaybeSword()) {
    return ETrue;
  }

  MaybeWalk();
  return ETrue;
}

TBool GPlayerProcess::WalkState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeSword()) {
    return ETrue;
  }

  if (MaybeSpell()) {
    return ETrue;
  }

  if (!MaybeWalk()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  mStepFrame++;
  if (mSprite->AnimDone() || (mState == WALK_STATE && mStepFrame - 1 >=
      TInt(WALK_SPEED * PLAYER_VELOCITY / hypot(mSprite->vx, mSprite->vy)))) {
    mStepFrame = 1;
    NewState(WALK_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GPlayerProcess::SwordState() {
  if (MaybeHit()) {
    mSprite->mSwordCharge = -1;
    return ETrue;
  }

  TFloat damageMultiplier;
  if (mStep < 3) {
    if (!GPlayer::mEquipped.mSword) {
      if (mStepFrame++ > SWORDSPEED) {
        mStep = 3;
      }
      return ETrue;
    }
    if (mStepFrame++ > SWORDSPEED && !gControls.IsPressed(CONTROL_FIRE)) {
      mStep = 3;
      return ETrue;
    }
    MaybeMove(PLAYER_VELOCITY * 0.2);
    if (mSprite->AnimDone() && (mSprite->vx != 0 || mSprite->vy != 0)) {
      NewState(SWORD_CHARGE_STATE, mSprite->mDirection);
    }
  }

  switch (mStep) {
    case 0:
      if (mSprite->mSwordCharge < 1.0) {
        mSprite->mSwordCharge += 1.0 / CHARGE_DURATION;
      } else {
        mSprite->mSwordCharge = 1.5;
        mStep++;
      }
      break;
    case 1:
      if (mStepFrame++ > PERFECT_CHARGE_WINDOW) {
        mSprite->mSwordCharge = 1.0;
        mStep++;
      }
      break;
    case 2:
      if (mSprite->mSwordCharge > 0.5) {
        mSprite->mSwordCharge -= 1.0 / CHARGE_DURATION;
      }
      break;
    case 3:
      NewState(SWORD_ATTACK_STATE, mSprite->mDirection);
      damageMultiplier = MAX(1.0, 1.0 + (CHARGE_BONUS - 1.0) * mSprite->mSwordCharge);
      if (mSprite->mSwordCharge >= 1.0) {
        mSprite->mSwordCharge = 2.0;
        damageMultiplier = PERFECT_CHARGE_BONUS;
      }

      mGameState->AddProcess(new GPlayerBulletProcess(mGameState, mSprite->mDirection, damageMultiplier));
      mStep++;
      break;
    case 4:
    default:
      if (mSprite->AnimDone()) {
        mSprite->mSwordCharge = -1;
        NewState(IDLE_STATE, mSprite->mDirection);
      }
      break;
  }

  return ETrue;
}

void GPlayerProcess::SpawnSpellProcesses() {
//  mSprite->x = mParent->mSprite->x + COS(aAngle) * aDistance * 1.5 + 16;
//  mSprite->y = mParent->mSprite->y + SIN(aAngle) * aDistance;
  switch (GPlayer::mEquipped.mSpellBookElement) {
    case ELEMENT_WATER: // Random locations around the player
      for (int i = 0; i < 15; ++i) {
        TInt16 spellX = mSprite->x + Random(-100, 100);
        TInt16 spellY = mSprite->y + 16 + Random(-100, 100);

        auto *p = new GSpellOverlayProcess(mGameState, this, spellX, spellY, Random(1, 30), 0, 0);
        mGameState->AddProcess(p);
      }
      break;
    case ELEMENT_FIRE:
      printf("ELEMENT_FIRE\n");

      mGameState->AddProcess(new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y, 0, -2.5 ,0));
      mGameState->AddProcess(new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y, 0, -2, -2));
      mGameState->AddProcess(new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y, 0, 0, 2.5));
      mGameState->AddProcess(new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y, 0, 2, 2));
      mGameState->AddProcess(new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y, 0, 0, -2.5));
      mGameState->AddProcess(new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y, 0, 2, -2));
      mGameState->AddProcess(new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y, 0, 2.5, 0));
      mGameState->AddProcess(new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y, 0, -2, 2));

      break;
    case ELEMENT_EARTH:
      printf("ELEMENT_EARTH\n");
      for (int i = 0; i < 5; ++i) {
        TInt16 spellX = mSprite->x + Random(-120, 120);
        TInt16 spellY = mSprite->y + 16 + Random(-120, 120);
        auto *p = new GSpellOverlayProcess(mGameState, this, spellX, spellY, Random(5, 40), 0 , 0);
        mGameState->AddProcess(p);
      }
      break;
    case ELEMENT_ENERGY:
      for (int i = 0; i < 5; ++i) {
        TInt16 spellX = mSprite->x + Random(-100, 100);
        TInt16 spellY = mSprite->y + 16 + Random(-100, 100);

        auto *p = new GSpellOverlayProcess(mGameState, this, spellX, spellY, 0, 0, 0);
//        auto *p = new GSpellOverlayProcess(mGameState, this, spellX, spellY, Random(5, 60), Random(-3, 3), Random(-3, 3));
        mGameState->AddProcess(p);
      }
      break;
    default:
      Panic("Invalid spell");
  }

}
TBool GPlayerProcess::SpellState() {
  switch (mStep) {
    case 0:
      if (mSprite->AnimDone()) {
        mStep++;
        mSprite2 = new GAnchorSprite(mGameState, PLAYER_SPELL_PRIORITY, PLAYER_SPELL_SLOT);
        mSprite2->x = mSprite->x + 16;
        mSprite2->y = mSprite->y + 1;
        mSprite2->StartAnimation(spellOverlayAnimation);
        gSoundPlayer.TriggerSfx(SFX_PLAYER_QUAFF_SPELL_WAV);
        mGameState->AddSprite(mSprite2);
        SpawnSpellProcesses();

        // affect nearby enemies
//        for (BSprite *s = mGameState->mSpriteList.First(); !mGameState->mSpriteList.End(s); s = mGameState->mSpriteList.Next(s)) {
//          if (!s->Clipped() && s->type == STYPE_ENEMY) {
//            TFloat dx = s->x - mSprite->x,
//                dy = s->y - mSprite->y,
//                distance = SQRT((dx * dx + dy * dy));
//            if (distance < SPELL_DISTANCE) {
//              s->SetCType(STYPE_SPELL);
//            }
//          }
//        }

      }
      break;
    case 1:
      if (mSprite2->AnimDone()) {
        mStep++;
        mSprite->StartAnimation(spell2Animation);
        mSprite2->Remove();
        delete mSprite2;
        mSprite2 = ENull;
      }
      break;
    case 2:
      if (mSprite->AnimDone()) {
        GPlayer::mInvulnerable = EFalse;
        NewState(IDLE_STATE, DIRECTION_DOWN);
//
        // affect nearby enemies
        for (BSprite *s = mGameState->mSpriteList.First(); !mGameState->mSpriteList.End(s); s = mGameState->mSpriteList.Next(s)) {
          if (!s->Clipped() && s->type == STYPE_ENEMY) {
            TFloat dx = s->x - mSprite->x,
                   dy = s->y - mSprite->y,
                   distance = SQRT((dx * dx + dy * dy));
            if (distance < SPELL_DISTANCE) {
              s->SetCType(STYPE_SPELL);
            }
          }
        }
      }
      break;
  }
  return ETrue;
}

TBool GPlayerProcess::FallState() {
  mStepFrame++;
  if (mStepFrame < FALL_DURATION) {
    mSprite->mDy = GRAVITY * TFloat(.5 * (mStepFrame - FALL_DURATION) * mStepFrame) - 4;
  } else if (mStepFrame == FALL_DURATION) {
    mSprite->StartAnimation(landAnimation);
  }

  if (mSprite->CanWalk(0, 0, ETrue)) {
    mSprite->vy = 0;
    if (mSprite->AnimDone()) {
      // land
      NewState(IDLE_STATE, mSprite->mDirection);
      return ETrue;
    }
  }

  return ETrue;
}

TBool GPlayerProcess::HitState() {
  // if player collides with another or the same enemy during knockback, reset direction
  if (mSprite->TestAndClearCType(STYPE_ENEMY | STYPE_EBULLET)) {
    StartKnockback();
  }

  if (mSprite->TestAndClearCType(STYPE_OBJECT)) {
    mSprite->Nudge();
  }

  if (mSprite->vx && !CanWalk(mSprite->vx, 0)) {
    mSprite->vx = 0;
  } else if (ABS(mSprite->vx) > PLAYER_FRICTION) {
    mSprite->vx -= PLAYER_FRICTION * (mSprite->vx / (ABS(mSprite->vx) + ABS(mSprite->vy)));
  }
  if (mSprite->vy && !CanWalk(0, mSprite->vy)) {
    mSprite->vy = 0;
  } else if (ABS(mSprite->vy) > PLAYER_FRICTION) {
    mSprite->vy -= PLAYER_FRICTION * (mSprite->vy / (ABS(mSprite->vx) + ABS(mSprite->vy)));
  }

  if (mSprite->AnimDone()) {
    mSprite->vx = mSprite->vy = 0;
    if (!GPlayer::mGameOver) {
      StartBlink(BLINK_TIME);
    }
    if (GPlayer::mGameOver || !MaybeWalk()) {
      NewState(IDLE_STATE, mSprite->mDirection);
    }
  }
  return ETrue;
}

TBool GPlayerProcess::RunBefore() {
  switch (mState) {
    case IDLE_STATE:
      return IdleState();
    case WALK_STATE:
      return WalkState();
    case SWORD_CHARGE_STATE:
    case SWORD_ATTACK_STATE:
      return SwordState();
    case FALL_STATE:
      return FallState();
    case HIT_HARD_STATE:
    case HIT_MEDIUM_STATE:
    case HIT_LIGHT_STATE:
      return HitState();
    case SPELL_STATE:
      return SpellState();
    default:
      return ETrue;
  }
}

TBool GPlayerProcess::RunAfter() {
  if (GPlayer::mTargeted) {
    if (GPlayer::mTargeted->Clipped() || !gControls.IsPressed(CONTROL_TARGET)) {
      GPlayer::mTargeted = ENull;
    }
  } else if (gControls.WasPressed(CONTROL_TARGET)) {
    if (GPlayer::mActiveBoss && GPlayer::mActiveBoss->mSprite &&
        !GPlayer::mActiveBoss->mSprite->Clipped()) {
      GPlayer::mTargeted = GPlayer::mActiveBoss->mSprite;
    } else if (GPlayer::mClosestEnemy && !GPlayer::mClosestEnemy->Clipped()) {
      GPlayer::mTargeted = GPlayer::mClosestEnemy;
    }
  }
  if (mBlinkTimer > 1) {
    mBlinkTimer--;
    if ((mBlinkTimer & 1u) == 0) {
      mSprite->ClearFlags(SFLAG_RENDER);
    } else {
      mSprite->SetFlags(SFLAG_RENDER);
    }
  } else if (mBlinkTimer == 1) {
    mBlinkTimer--;
    mSprite->SetFlags(SFLAG_RENDER);
    GPlayer::mInvulnerable = EFalse;
  }
  mSprite->cType = 0;

  return ETrue;
}

void GPlayerProcess::WriteCustomToStream(BMemoryStream &aStream) {
  aStream.Write(&mState, sizeof(mState));
  aStream.Write(&mStep, sizeof(mStep));
  //  TBool v = mBlinkProcess ? ETrue : EFalse;
  //  aStream.Write(&v, sizeof(v));
}

void GPlayerProcess::ReadCustomFromStream(BMemoryStream &aStream) {
  aStream.Read(&mState, sizeof(mState));
  aStream.Read(&mStep, sizeof(mStep));
  //  TBool v = mBlinkProcess ? ETrue : EFalse;
  //  aStream.Read(&v, sizeof(v));
}
