#include "GPlayerProcess.h"
#include "GPlayer.h"
#include "GPlayerSprite.h"
#include "GGamePlayfield.h"
#include "GPlayerAnimations.h"
#include "GStatProcess.h"
#include "GResources.h"
#include "GPlayerBulletProcess.h"

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

// To make the player blink and be invulnerable, a second Process is spawned to
// countdown the invulnerable time.  At the start, the player's mSprite is set to
// invulnerable.  As the countdown happens, the SFLAG_RENDER flag is toggled to
// cause a blinking effect
const TInt16 BLINK_TIME = FRAMES_PER_SECOND * 0.6;

void GPlayerProcess::BlinkOn() {
  GPlayer::mSprite->ClearFlags(SFLAG_RENDER);
}

void GPlayerProcess::BlinkOff() {
  GPlayer::mSprite->SetFlags(SFLAG_RENDER);
}

TFloat GPlayerProcess::PlayerX() { return mSprite->x; }

TFloat GPlayerProcess::PlayerY() { return mSprite->y; }

DIRECTION GPlayerProcess::mLastDirection = DIRECTION_DOWN;
TFloat GPlayerProcess::mRespawnAt[2] = { '\0', '\0' };

GPlayerProcess::GPlayerProcess(GGameState *aGameState) : GLivingProcess(ATTR_PLAYER_IN1) {
  mState = IDLE_STATE;
  mStep = 0;
  mStepFrame = 0;
  mGameState = aGameState;
  mPlayfield = ENull;
  GPlayer::mSprite = mSprite = ENull;

  // initialize player sprite
  GPlayer::mSprite = mSprite = new GPlayerSprite(mGameState);
  mGameState->AddSprite(mSprite);

  mSprite2 = ENull;

  NewState(IDLE_STATE, mLastDirection);
}

GPlayerProcess::~GPlayerProcess() {
  if (mSprite2) {
    mSprite2->Remove();
    delete mSprite2;
    mSprite2 = ENull;
  }

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
  GAnchorSprite *other = mSprite->mCollided;
  if (other && other->TestFlags(SFLAG_KNOCKBACK)) {
    // push player away from center of other sprite's hit box
    TRect myRect, otherRect;
    mSprite->GetRect(myRect);
    other->GetRect(otherRect);
    TFloat dx = (mSprite->x + myRect.x1 + TFloat(myRect.Width()) / 2) - (other->x + otherRect.x1 + TFloat(otherRect.Width()) / 2),
            dy = (mSprite->y + myRect.y1 + TFloat(myRect.Height()) / 2) - (other->y + otherRect.y1 + TFloat(otherRect.Height()) / 2);
    TFloat newVx, newVy;

    newVx = PLAYER_VELOCITY * dx / hypot(dx, dy);
    newVy = PLAYER_VELOCITY * dy / hypot(dx, dy);

    // if other sprite is moving towards player, add its momentum to player knockback
    if (dx > 0 ^ other->vx < 0) {
      newVx += other->vx;
    }
    if (dy > 0 ^ other->vy < 0) {
      newVy += other->vy;
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
        mStep = 1 - mStep;
//        printf("walkAnimation %i, direction %i\n", mStep ? 1 : 2, aDirection);
        mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
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

      mInvulnerable = ETrue;
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

  if (mInvulnerable) {
    mSprite->cType = 0;
    return EFalse;
  }

  GAnchorSprite *other = mSprite->mCollided;

  if (other) {
    TInt hitAmount = 0;

    if (mSprite->TestAndClearCType(STYPE_EBULLET)) {
      hitAmount = other->mAttackStrength;
      if (hitAmount <= GPlayer::mMaxHitPoints * 0.15) {
        mSprite->StartAnimationInDirection(hitLightAnimations, GAnchorSprite::RotateDirection(other->mDirection, 2));
      }
      else if (hitAmount <= GPlayer::mMaxHitPoints * 0.30) {
        mSprite->StartAnimationInDirection(hitMediumAnimations, GAnchorSprite::RotateDirection(other->mDirection, 2));
      }
      else {
        mSprite->StartAnimationInDirection(hitHardAnimations, GAnchorSprite::RotateDirection(other->mDirection, 2));
      }
    }

    if (mSprite->TestAndClearCType(STYPE_ENEMY)) {
      if (other->mHitPoints > 0) {
        // contact damage independent of enemy attack strength
        hitAmount = BASE_STRENGTH + other->mLevel * (BASE_STRENGTH / 5);
        mSprite->StartAnimationInDirection(hitLightAnimations, mSprite->mDirection);
      }
    }

    if (hitAmount) {
      TInt state = HIT_LIGHT_STATE;
      // Random +/- 20% variation
      hitAmount = (hitAmount * Random(80, 120)) / 100;


      if (GPlayer::mEquipped.mAmuletElement && other->mElement) {
        hitAmount *= AMULET_MATRIX[GPlayer::mEquipped.mAmuletElement - 1][other->mElement - 1];
      }

      GPlayer::mHitPoints -= hitAmount;
      mInvulnerable = ETrue;
      auto *p = new GStatProcess(mSprite->x + 72, mSprite->y + 32, "%d", hitAmount);
      p->SetMessageType(STAT_PLAYER_HIT);
      mGameState->AddProcess(p);

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
        }
      }

      StartKnockback();

      mSprite->cType = 0;
      return ETrue;
    }
  } else {
    mSprite->TestAndClearCType(STYPE_ENEMY | STYPE_EBULLET);
  }

  if (mSprite->TestAndClearCType(STYPE_OBJECT)) {
    NewState(IDLE_STATE, mSprite->mDirection);
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

  mSprite->vy = newVy;
  mSprite->vx = newVx;

  return newDirection;
}

TBool GPlayerProcess::MaybeWalk() {
  if (GPlayer::mGameOver) {
    return ETrue;
  }

  if (MaybeFall()) {
    return EFalse;
  }

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
    NewState(IDLE_STATE, newDirection);
    return EFalse;
  }
  else if (mState != WALK_STATE || mSprite->mDirection != newDirection) {
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
  if (mSprite->AnimDone() || (mState == WALK_STATE && mStepFrame - 1 >= (WALKSPEED * 2) * PLAYER_VELOCITY / SQRT(POW(mSprite->vx, 2) + POW(mSprite->vy, 2)))) {
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
    if (!gControls.IsPressed(CONTROL_FIRE)) {
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

TBool GPlayerProcess::SpellState() {
  switch (mStep) {
    case 0:
      if (mSprite->AnimDone()) {
        mStep++;
        mSprite2 = new GAnchorSprite(mGameState, PLAYER_SPELL_PRIORITY, PLAYER_SPELL_SLOT);
        mSprite2->x = mSprite->x + 16;
        mSprite2->y = mSprite->y + 1;
        mSprite2->StartAnimation(spellOverlayAnimation);
//        printf("SPELLLL\n");
        gSoundPlayer.TriggerSfx(SFX_PLAYER_QUAFF_SPELL_WAV);
        mGameState->AddSprite(mSprite2);
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
        mInvulnerable = EFalse;
        NewState(IDLE_STATE, DIRECTION_DOWN);

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
  GLivingProcess::RunBefore();
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
  GLivingProcess::RunAfter();
  mSprite->mCollided = ENull;
  mSprite->cType = 0;

  return ETrue;
}

void GPlayerProcess::WriteToStream(BMemoryStream &aStream) {
  // process vars
  aStream.Write(&mState, sizeof(mState));
  aStream.Write(&mStep, sizeof(mStep));
}

void GPlayerProcess::WriteCustomToStream(BMemoryStream &aStream) {
  aStream.Write(&mState, sizeof(mState));
  aStream.Write(&mStep, sizeof(mStep));
  //  TBool v = mBlinkProcess ? ETrue : EFalse;
  //  aStream.Write(&v, sizeof(v));
}

void GPlayerProcess::ReadFromStream(BMemoryStream &aStream) {
  // process vars
  aStream.Read(&mState, sizeof(mState));
  aStream.Read(&mStep, sizeof(mStep));
}

void GPlayerProcess::ReadCustomFromStream(BMemoryStream &aStream) {
  aStream.Read(&mState, sizeof(mState));
  aStream.Read(&mStep, sizeof(mStep));
  //  TBool v = mBlinkProcess ? ETrue : EFalse;
  //  aStream.Read(&v, sizeof(v));
}
