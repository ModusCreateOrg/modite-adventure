#include "GPlayerProcess.h"
#include "GPlayer.h"
#include "GAnchorSprite.h"
#include "GGamePlayfield.h"
#include "GPlayerAnimations.h"
#include "GStatProcess.h"
#include "GResources.h"
#include "Items.h"
#include "GPlayerBulletProcess.h"

#define DEBUGME
#undef DEBUGME

const TFloat SPELL_DISTANCE = 200.0;

const TUint16 IDLE_STATE = 0;
const TUint16 WALK_STATE = 1;
const TUint16 SWORD_STATE = 2;
const TUint16 SWORD_NO_BULLET_STATE = 3;
const TUint16 FALL_STATE = 4;
const TUint16 HIT_LIGHT_STATE = 5;
const TUint16 HIT_MEDIUM_STATE = 6;
const TUint16 HIT_HARD_STATE = 7;
const TUint16 QUAFF_STATE = 8;
const TUint16 SPELL_STATE = 9;

// To make the player blink and be invulnerable, a second Process is spawned to
// countdown the invulnerable time.  At the start, the player's mSprite is set to
// involunerable.  As the countdown happens, the SFLAG_RENDER flag is toggled to
// cause a blinking effect
const TInt16 BLINK_TIME = 16; // 267ms

class GPlayerBlinkProcess : public GProcess {
public:
  GPlayerBlinkProcess() : GProcess(ATTR_GONE) {
    GPlayer::mSprite->mInvulnerable = ETrue;
    mTimer = BLINK_TIME;
  }
  ~GPlayerBlinkProcess() {
    if (GPlayer::mSprite) {
      GPlayer::mSprite->mInvulnerable = EFalse;
      GPlayer::mSprite->SetFlags(SFLAG_RENDER);
    }
    if (GPlayer::mProcess) {
      GPlayer::mProcess->mBlinkProcess = ENull;
    }
  }

public:
  void Kill() {
    mTimer = 1;
  }

public:
  TBool RunBefore() {
    if (--mTimer <= 0 || !GPlayer::mSprite) {
      return EFalse;
    }
    if ((mTimer & 1) == 0) {
      GPlayer::mSprite->SetFlags(SFLAG_RENDER);
    }
    else {
      GPlayer::mSprite->ClearFlags(SFLAG_RENDER);
    }
    return ETrue;
  }

  TBool RunAfter() {
    return ETrue;
  }

private:
  TInt mTimer;
};

TFloat GPlayerProcess::PlayerX() { return mSprite->x; }

TFloat GPlayerProcess::PlayerY() { return mSprite->y; }

DIRECTION GPlayerProcess::mLastDirection = DIRECTION_DOWN;
GPlayerProcess::GPlayerProcess(GGameState *aGameState) : GProcess(ATTR_PLAYER_IN1) {
  mState = IDLE_STATE;
  mStep = 0;
  mStepFrame = 0;
  mMomentum = 0.0;
  mGameState = aGameState;
  mPlayfield = ENull;
  mBlinkProcess = ENull;
  GPlayer::mSprite = mSprite = ENull;

  // initialize player sprite
  GPlayer::mSprite = mSprite = new GAnchorSprite(mGameState, PLAYER_PRIORITY, PLAYER_SLOT);
  mSprite->Name("PLAYER");
  mSprite->type = STYPE_PLAYER;
  mSprite->SetCMask(STYPE_ENEMY | STYPE_EBULLET | STYPE_OBJECT); // collide with enemy, enemy attacks, and environment
  mSprite->w = 26;
  mSprite->h = 16;
  mSprite->cx = 7;
  mSprite->cy = 0;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_HERO_BMP_SPRITES);
  mGameState->AddSprite(mSprite);
  mSprite->SetFlags(SFLAG_ANCHOR | SFLAG_CHECK | SFLAG_RENDER_SHADOW); // SFLAG_SORTY

  mSprite2 = ENull;

  NewState(IDLE_STATE, mLastDirection);
}

GPlayerProcess::~GPlayerProcess() {
  if (mBlinkProcess) {
    mBlinkProcess->Kill();
    mBlinkProcess = ENull;
  }
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

    if (aExitingLevel == 0) {
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
      printf("OVERWORLD ENTRANCE row,col = %d,%d params = %d/%x %d\n", row, col, params, params, dungeon);
      printf("GetMapHeight() = %i, GetMapWidth() = %i\n", mPlayfield->GetMapHeight(), mPlayfield->GetMapWidth());

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
        return;
      }
    }
    Panic("Could not find dungeon entrance %d\n", aExitingDungeon);
  }
}

TBool GPlayerProcess::IsLedge() {
  TRect r;
  mSprite->GetRect(r);

  return mPlayfield->IsLedge(r.x1 + FLOOR_ADJUST_LEFT, r.y2) ||
         mPlayfield->IsLedge(r.x2 - FLOOR_ADJUST_RIGHT, r.y2);
}

TBool GPlayerProcess::CanWalk(DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      return mSprite->IsFloor(DIRECTION_UP, 0, -PLAYER_VELOCITY);
    case DIRECTION_DOWN:
      return mSprite->IsFloor(DIRECTION_DOWN, 0, PLAYER_VELOCITY);
    case DIRECTION_LEFT:
      return mSprite->IsFloor(DIRECTION_LEFT, -PLAYER_VELOCITY, 0);
    case DIRECTION_RIGHT:
    default:
      return mSprite->IsFloor(DIRECTION_RIGHT, PLAYER_VELOCITY, 0);
  }
}

void GPlayerProcess::StartKnockback() {
  GAnchorSprite *other = mSprite->mCollided;
  if (other && other->TestFlags(SFLAG_KNOCKBACK)) {
    // push player away from center of other sprite's hit box
    TRect myRect, otherRect;
    mSprite->GetRect(myRect);
    other->GetRect(otherRect);
    TFloat velocity = PLAYER_VELOCITY;
    TFloat dx = (mSprite->x+myRect.x1+myRect.Width()) - (other->x+otherRect.x1+otherRect.Width()),
            dy = (mSprite->y+myRect.y1+myRect.Height()) - (other->y+otherRect.y1+otherRect.Height());

    // if other sprite is moving towards player, add its momentum to player knockback
    if (dx > 0 ^ other->vx < 0) {
      velocity += ABS(other->vx);
    }
    if (dy > 0 ^ other->vy < 0) {
      velocity += ABS(other->vy);
    }

    if ((dx < 0 && CanWalk(DIRECTION_LEFT)) ||
        (dx > 0 && CanWalk(DIRECTION_RIGHT))) {
      mSprite->vx = velocity * (dx / (ABS(dx) + ABS(dy)));
    }
    if ((dy < 0 && CanWalk(DIRECTION_UP)) ||
        (dy > 0 && CanWalk(DIRECTION_DOWN))) {
      mSprite->vy = velocity * (dy / (ABS(dx) + ABS(dy)));
    }
  }
}

void GPlayerProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;

  mLastDirection = aDirection;

  mSprite->mDx = 0;
  mSprite->mDy = 0;
  switch (mState) {

    case WALK_STATE:
      if (mStepFrame > 0) {
        switch (mSprite->mDirection) {
          case DIRECTION_UP:
            mStep = 1 - mStep;
            mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
            break;
          case DIRECTION_DOWN:
            mStep = 1 - mStep;
            mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
            break;
          case DIRECTION_LEFT:
            mStep = 1 - mStep;
            mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
            break;
          case DIRECTION_RIGHT:
            mStep = 1 - mStep;
            mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
            break;
        }
        break;
      }
    case IDLE_STATE:
      mStep = 0;
      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          if (mMomentum > 0.5) {
            mSprite->StartAnimation(skidUpAnimation);
          }
          else {
            mSprite->StartAnimation(idleUpAnimation);
          }
          break;
        case DIRECTION_DOWN:
          if (mMomentum > 0.5) {
            mSprite->StartAnimation(skidDownAnimation);
          }
          else {
            mSprite->StartAnimation(idleDownAnimation);
          }
          break;
        case DIRECTION_LEFT:
          if (mMomentum > 0.5) {
            mSprite->StartAnimation(skidLeftAnimation);
          }
          else {
            mSprite->StartAnimation(idleLeftAnimation);
          }
          break;
        case DIRECTION_RIGHT:
          if (mMomentum > 0.5) {
            mSprite->StartAnimation(skidRightAnimation);
          }
          else {
            mSprite->StartAnimation(idleRightAnimation);
          }
          break;
      }
      break;
    case SWORD_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;

      gSoundPlayer.SfxPlayerSlash();
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

    case FALL_STATE:
      mStep = 0;
      mStepFrame = 0;
      mSprite->vx = 0;
      mSprite->vy = PLAYER_VELOCITY;
      mSprite->mDy = 0;
      mSprite->StartAnimation(fallAnimation);
      mSprite->mDirection = DIRECTION_DOWN;
      break;

    case QUAFF_STATE:
      gSoundPlayer.SfxPlayerQuaffHealthPotion();
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;
      mSprite->StartAnimation(quaff1Animation);
      mSprite->mDirection = DIRECTION_DOWN;
      break;

    case SPELL_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      mStep = 0;

      mSprite->mInvulnerable = ETrue;
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

TBool GPlayerProcess::MaybeQuaff() {
  if (GPlayer::mGameOver) {
    return EFalse;
  }
  if (gControls.WasPressed(CONTROL_QUAFF)) {
    if (GPlayer::mHealthPotion > 0) {
      GPlayer::mHealthPotion -= 25;
      NewState(QUAFF_STATE, DIRECTION_DOWN);
    }
    return ETrue;
  }
  return EFalse;
}

TBool GPlayerProcess::MaybeSpell() {
  if (GPlayer::mGameOver) {
    return EFalse;
  }
  if (gControls.WasPressed(CONTROL_SPELL)) {
    if (GPlayer::mManaPotion > 0 && GPlayer::mEquipped.mSpellbook) {
      if (GPlayer::mManaPotion >= 25) {
        GPlayer::mManaPotion -= 25;
      }
      NewState(SPELL_STATE, DIRECTION_DOWN);
    }
    return ETrue;
  }
  return EFalse;
}

TBool GPlayerProcess::MaybeHit() {
  if (mSprite->TestCType(STYPE_EBULLET | STYPE_ENEMY | STYPE_OBJECT)) {
    mSprite->Nudge();
    mMomentum = 0;
  }

  if (mSprite->mInvulnerable) {
    mSprite->cType = 0;
    return EFalse;
  }

  GAnchorSprite *other = mSprite->mCollided;

  if (other) {
    TInt hitAmount = 0;

    if (mSprite->TestAndClearCType(STYPE_EBULLET)) {
      // random variation from 100% to 150% base damage
      hitAmount = other->mHitStrength + round(RandomFloat() * other->mHitStrength / 2);
      if (hitAmount <= GPlayer::mMaxHitPoints * 0.15) {
        switch (other->mDirection) {
          case DIRECTION_UP:
            mSprite->StartAnimation(hitLightDownAnimation);
            break;
          case DIRECTION_DOWN:
            mSprite->StartAnimation(hitLightUpAnimation);
            break;
          case DIRECTION_LEFT:
            mSprite->StartAnimation(hitLightRightAnimation);
            break;
          case DIRECTION_RIGHT:
          default:
            mSprite->StartAnimation(hitLightLeftAnimation);
            break;
        }
      }
      else if (hitAmount <= GPlayer::mMaxHitPoints * 0.30) {
        switch (other->mDirection) {
          case DIRECTION_UP:
            mSprite->StartAnimation(hitMediumDownAnimation);
            break;
          case DIRECTION_DOWN:
            mSprite->StartAnimation(hitMediumUpAnimation);
            break;
          case DIRECTION_LEFT:
            mSprite->StartAnimation(hitMediumRightAnimation);
            break;
          case DIRECTION_RIGHT:
          default:
            mSprite->StartAnimation(hitMediumLeftAnimation);
            break;
        }
      }
      else {
        switch (other->mDirection) {
          case DIRECTION_UP:
            mSprite->StartAnimation(hitHardDownAnimation);
            break;
          case DIRECTION_DOWN:
            mSprite->StartAnimation(hitHardUpAnimation);
            break;
          case DIRECTION_LEFT:
            mSprite->StartAnimation(hitHardRightAnimation);
            break;
          case DIRECTION_RIGHT:
          default:
            mSprite->StartAnimation(hitHardLeftAnimation);
            break;
        }
      }
    }

    if (mSprite->TestAndClearCType(STYPE_ENEMY)) {
      // random variation from 50% to 100% base damage
      if (other->mHitPoints > 0) {
        hitAmount = other->mHitStrength - round(RandomFloat() * other->mHitStrength / 2);
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
          default:
            mSprite->StartAnimation(hitLightRightAnimation);
            break;
        }
      }
    }

    if (hitAmount) {
      TInt state = HIT_LIGHT_STATE;

      // random variation from 100% to 150% base damage
      GPlayer::mHitPoints -= hitAmount;
      mSprite->mInvulnerable = ETrue;
      auto *p = new GStatProcess(mSprite->x + 72, mSprite->y + 32, "%d", hitAmount);
      p->SetMessageType(STAT_PLAYER_HIT);
      mGameState->AddProcess(p);

      if (!mBlinkProcess) {
        mGameState->AddProcess(mBlinkProcess = new GPlayerBlinkProcess());
      }

      gSoundPlayer.SfxPlayerTakeDamage();

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
  if (!gControls.WasPressed(CONTROL_FIRE)) {
    return EFalse;
  }

  auto *p = (GProcess *)new GPlayerBulletProcess(mGameState, mSprite->mDirection);
  mGameState->AddProcess(p);
  NewState(SWORD_STATE, mSprite->mDirection);
  return ETrue;
}

TBool GPlayerProcess::MaybeFall() {
  if (IsLedge()) {
    NewState(FALL_STATE, DIRECTION_DOWN);
    return ETrue;
  }
  return EFalse;
}

TBool GPlayerProcess::MaybeWalk() {
  if (GPlayer::mGameOver) {
    return ETrue;
  }
  TFloat newVx = 0.0, newVy = 0.0;
  DIRECTION newDirection = mSprite->mDirection;
  if (gControls.IsPressed(CONTROL_JOYUP)) {
    newVy = -PLAYER_VELOCITY;
    newDirection = DIRECTION_UP;
  }
  else if (gControls.IsPressed(CONTROL_JOYDOWN)) {
    newVy = PLAYER_VELOCITY;
    newDirection = DIRECTION_DOWN;
  }
  if (gControls.IsPressed(CONTROL_JOYLEFT)) {
    if (ABS(newVy) > 0) {
      newVx = -PLAYER_VELOCITY * sqrt(2) / 2;
      newVy = newVy * sqrt(2) / 2;
    }
    else {
      newVx = -PLAYER_VELOCITY;
    }
    if (newVy == 0 || newVx != 0) {
      newDirection = DIRECTION_LEFT;
    }
  }
  else if (gControls.IsPressed(CONTROL_JOYRIGHT)) {
    if (ABS(newVy) > 0) {
      newVx = PLAYER_VELOCITY * sqrt(2) / 2;
      newVy = newVy * sqrt(2) / 2;
    }
    else {
      newVx = PLAYER_VELOCITY;
    }
    if (newVy == 0 || newVx != 0) {
      newDirection = DIRECTION_RIGHT;
    }
  }

  if (newVy > 0.0 && MaybeFall()) {
    return EFalse;
  }

  if (((mSprite->vy < 0 || newVy < 0) && !CanWalk(DIRECTION_UP)) ||
      ((mSprite->vy > 0 || newVy > 0) && !CanWalk(DIRECTION_DOWN))) {
    mSprite->vy = 0;
    newVy = 0;
  }
  if (((mSprite->vx < 0 || newVx < 0) && !CanWalk(DIRECTION_LEFT)) ||
      ((mSprite->vx > 0 || newVx > 0) && !CanWalk(DIRECTION_RIGHT))) {
    mSprite->vx = 0;
    newVx = 0;
  }

  if (gControls.IsPressed(CONTROL_RUN) && (newVx != 0 || newVy != 0)) {
    if (GPlayer::mEquipped.mBoots && GPlayer::mEquipped.mBoots->mItemNumber == ITEM_BOOTS) {
      if (mMomentum < 1.3) {
        mMomentum += PLAYER_FRICTION / 4;
      }
    }
    else {
      if (mMomentum < 0.3) {
        mMomentum += PLAYER_FRICTION / 4;
      }
    }
  }
  else {
    if (mMomentum > 0) {
      mMomentum -= PLAYER_FRICTION;
    }
    if (mMomentum < 0) {
      mMomentum = 0;
    }
  }

  newVy *= (1 + mMomentum);
  newVx *= (1 + mMomentum);
  if (newVx == 0 && newVy == 0) {
    if (mMomentum > 0) {
      if (mSprite->vy > PLAYER_VELOCITY) {
        newVy = mSprite->vy - PLAYER_FRICTION;
      }
      else if (mSprite->vy < -PLAYER_VELOCITY) {
        newVy = mSprite->vy + PLAYER_FRICTION;
      }
      if (mSprite->vx > PLAYER_VELOCITY) {
        newVx = mSprite->vx - PLAYER_FRICTION;
      }
      else if (mSprite->vx < -PLAYER_VELOCITY) {
        newVx = mSprite->vx + PLAYER_FRICTION;
      }
    }
    if (mState != IDLE_STATE || mSprite->mDirection != newDirection) {
      NewState(IDLE_STATE, newDirection);
    }
  }
  else {
    if (mState != WALK_STATE || mSprite->mDirection != newDirection) {
      NewState(WALK_STATE, newDirection);
    }
  }

  mSprite->vy = newVy;
  mSprite->vx = newVx;

  if (mSprite->vx == 0 && mSprite->vy == 0) {
    mMomentum = 0;
    NewState(IDLE_STATE, newDirection);
    return EFalse;
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

  if (MaybeQuaff()) {
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

  if (MaybeQuaff()) {
    return ETrue;
  }

  if (!MaybeWalk()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  mStepFrame++;
  if (mSprite->AnimDone() || (mState == WALK_STATE && mStepFrame - 1 >= (WALKSPEED * 2) / (1 + mMomentum))) {
    mStepFrame = 1;
    NewState(WALK_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GPlayerProcess::SwordState() {
  if (MaybeHit()) {
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GPlayerProcess::QuaffState() {
  switch (mStep) {
    case 0:
      if (mSprite->AnimDone()) {
        mStep++;
        mSprite2 = new GAnchorSprite(mGameState, PLAYER_HEAL_PRIORITY, PLAYER_HEAL_SLOT);
        mSprite2->x = mSprite->x + 16;
        mSprite2->y = mSprite->y + 1;
        mSprite2->StartAnimation(quaffOverlayAnimation);
        mGameState->AddSprite(mSprite2);
      }
      break;
    case 1:
      if (mSprite2->AnimDone()) {
        mStep++;
        mSprite->StartAnimation(quaff2Animation);
        mSprite2->Remove();
        delete mSprite2;
        mSprite2 = ENull;
      }
      break;
    case 2:
      if (mSprite->AnimDone()) {
        NewState(IDLE_STATE, DIRECTION_DOWN);
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
        mSprite->mInvulnerable = EFalse;
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
    mSprite->mDy = GRAVITY * TFloat(.5 * (mStepFrame - FALL_DURATION) * mStepFrame);
  } else if (mStepFrame == FALL_DURATION) {
    mSprite->StartAnimation(landAnimation);
  }

  if (mSprite->IsFloor(DIRECTION_UP, 0, 0) &&
      mSprite->IsFloor(DIRECTION_DOWN, 0, 0)) {
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

  if ((mSprite->vx < 0 && !CanWalk(DIRECTION_LEFT)) ||
      (mSprite->vx > 0 && !CanWalk(DIRECTION_RIGHT))) {
    mSprite->vx = 0;
  } else if (ABS(mSprite->vx) > PLAYER_FRICTION) {
    mSprite->vx -= PLAYER_FRICTION * (mSprite->vx / (ABS(mSprite->vx) + ABS(mSprite->vy)));
  }
  if ((mSprite->vy < 0 && !CanWalk(DIRECTION_UP)) ||
      (mSprite->vy > 0 && !CanWalk(DIRECTION_DOWN))) {
    mSprite->vy = 0;
  } else if (ABS(mSprite->vy) > PLAYER_FRICTION) {
    mSprite->vy -= PLAYER_FRICTION * (mSprite->vy / (ABS(mSprite->vx) + ABS(mSprite->vy)));
  }

  if (mSprite->AnimDone()) {
    mSprite->vx = mSprite->vy = 0;
    if (!GPlayer::mGameOver && !mBlinkProcess) {
      mGameState->AddProcess(mBlinkProcess = new GPlayerBlinkProcess());
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
    case SWORD_STATE:
      return SwordState();
    case FALL_STATE:
      return FallState();
    case HIT_HARD_STATE:
    case HIT_MEDIUM_STATE:
    case HIT_LIGHT_STATE:
      return HitState();
    case QUAFF_STATE:
      return QuaffState();
    case SPELL_STATE:
      return SpellState();
    default:
      return ETrue;
  }
}

TBool GPlayerProcess::RunAfter() {
  // position viewport to follow player
  TFloat maxx = mGameState->MapWidth(),
         maxy = mGameState->MapHeight();

  // half viewport size
  const TFloat ww = gViewPort->mRect.Width() / 2.0,
               hh = gViewPort->mRect.Height() / 2.0;

  // upper left corner of desired viewport position
  TFloat xx = gViewPort->mWorldX = mSprite->x - ww,
         yy = gViewPort->mWorldY = mSprite->y - hh;

  if (xx < 0) {
    gViewPort->mWorldX = 0;
  }
  else if (xx > maxx) {
    gViewPort->mWorldX = maxx;
  }
  if (yy < 0) {
    gViewPort->mWorldY = 0;
  }
  else if (yy > maxy) {
    gViewPort->mWorldY = maxy;
  }

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
