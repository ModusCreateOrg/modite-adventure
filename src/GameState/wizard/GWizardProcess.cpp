#include <math.h>
#include "GWizardProcess.h"
#include "GWizardProjectileProcess.h"
#include "GWizardPillarProcess.h"
#include "GWizardDeathProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"
#include "GItemProcess.h"
#include "GWizardDecoyProcess.h"
#include "Game.h"

#define DEBUGME
//#undef DEBUGME

const TInt16 IDLE_SPEED = 4 * FACTOR;
const TInt16 WALK_SPEED = 4 * FACTOR;
const TFloat WALK_VELOCITY = 1.25;
const TInt16 ATTACK_SPEED = 2 * FACTOR;
const TInt HIT_SPAM_TIME = 2 * FRAMES_PER_SECOND;
const TInt HEAL_RATE = 100; // amount to heal per three seconds during illusion spell

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(1, IMG_WIZARD_WALK_DOWN + 3),
  AEND,
};

static ANIMSCRIPT channelingAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  ASTEP(ATTACK_SPEED * 6, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED * 4, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE + 1),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_DOWN),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_UP),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  AFLIP(WALK_SPEED, IMG_WIZARD_WALK_LEFT),
  AFLIP(WALK_SPEED, IMG_WIZARD_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  AFLIP(WALK_SPEED, IMG_WIZARD_WALK_LEFT + 2),
  AFLIP(WALK_SPEED, IMG_WIZARD_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_RIGHT),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_WIZARD_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(IDLE_SPEED, IMG_WIZARD_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT projectileAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  ASTEP(ATTACK_SPEED * 6, IMG_WIZARD_FIRE_START),
  ALABEL,
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 4),
  ALOOP,
};

static ANIMSCRIPT projectileAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE_START),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  AEND,
};

static ANIMSCRIPT teleportAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE_START),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  AEND,
};

static ANIMSCRIPT teleportAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED * 2, IMG_WIZARD_FIRE_START),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  AEND,
};

static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};

// constructor
GWizardProcess::GWizardProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TInt aIp, TInt aType, TUint16 aAttribute, TUint16 aSpriteSheet)
    : GLivingProcess(aAttribute) {
  printf("GWizardProcess(attribute: %d/$%x)\n", aAttribute, aAttribute);
  mGameState = aGameState;
  mSlot = aSlot;
  mIp = aIp;
  mType = aType;
  mAttribute = aAttribute;
  mStartX = aX;
  mStartY = aY;
  //
  mSaveToStream = ETrue;
  //
  mSprite = new GAnchorSprite(mGameState, 0, aSlot);
  switch (mType) {
    case ATTR_WIZARD_EARTH:
      mSprite->Name("Ikuyim");
      break;
    case ATTR_WIZARD_WATER:
      mSprite->Name("Asakust");
      break;
    case ATTR_WIZARD_FIRE:
      mSprite->Name("Imagak");
      break;
    case ATTR_WIZARD_ENERGY:
      mSprite->Name("Atanok");
      break;
    default:
      Panic("Invalid wizard type");
  }
  mSpriteSheet = aSpriteSheet;
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(aSpriteSheet);
  mSprite->type = STYPE_ENEMY;
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT);
  mSprite->SetFlags(SFLAG_RENDER_SHADOW | SFLAG_KNOCKBACK | SFLAG_CHECK);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->mDy = -4;
  mSprite->cx = -4;
  mSprite->cy = 0;
  mSprite->w = 36;
  mSprite->h = 20;
  mSprite->SetStatMultipliers(10.0, 3.0, 10.0);
  mGameState->AddSprite(mSprite);
  //
  mHitTimer = HIT_SPAM_TIME;
  mStateTimer = 2 * 60;
  mStep = 0;
  mDeathCounter = 0;
  mSpellCounter = 0;
  mAttackType = 0;
  SetState(STATE_IDLE, DIRECTION_DOWN);
  SetAttackTimer();
  GPlayer::mActiveBoss = mSprite;
  mChanneling = EFalse;
}

GWizardProcess::~GWizardProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
  GPlayer::mActiveBoss = ENull;
  GGameState *gameState = (GGameState*)gGameEngine;
  gameState->PlayMusicForCurrentLevel();
}

void GWizardProcess::SetAttackTimer() {
//  mAttackTimer = 5;
  mAttackTimer = Random(2 * FRAMES_PER_SECOND, 3 * FRAMES_PER_SECOND);
}

void GWizardProcess::RandomLocation() {
  mChanneling = EFalse;
  TInt nTries = 0;
  // searches random directions from the wizard's spawn point,
  // tries 10 times to move to a location not near the player
  do {
    mSprite->x = mStartX;
    mSprite->y = mStartY;
    TFloat dx = RandomFloat() * 8 - 4;
    TFloat dy = (Random() & 1u) ? 4 : -4;
    if (Random() & 1u) {
      TFloat tmp = dx;
      dx = dy;
      dy = tmp;
    }
    TInt i = 0;
    while (mSprite->CanWalk(dx * i, dy * i, ETrue)) {
      i++;
    }
    mSprite->x = mStartX + dx * i * SQRT(RandomFloat()) * 0.8;
    mSprite->y = mStartY + dy * i * SQRT(RandomFloat()) * 0.8;
  } while (nTries++ < 10 && (ABS(mSprite->x - GPlayer::mSprite->mLastX) < 96 && ABS(mSprite->y - GPlayer::mSprite->mLastY) < 96));
}

// start wizard idle in specified direction
void GWizardProcess::Idle(DIRECTION aDirection) {
  mDirection = aDirection;
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(idleAnimation);
  mInvulnerable = EFalse;
  mStateTimer = FRAMES_PER_SECOND * 2;
}

// start wizard walking in specified direction
void GWizardProcess::Walk(DIRECTION aDirection) {
  mDirection = aDirection;
  mStep = 1 - mStep;
  mSprite->StartAnimationInDirection(mStep ? walkAnimations2 : walkAnimations1, aDirection);
  mSprite->vx = mSprite->vy = 0;
  mSprite->MoveInDirection(WALK_VELOCITY, aDirection);
}

void GWizardProcess::Projectile() {
  printf("Projectile\n");
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(projectileAnimation1);
  mStep = 0;
  mStateTimer = FRAMES_PER_SECOND * 0.5;
  mAttackType = Random(0, 2);
  mChanneling = mAttackType != 1;
}

void GWizardProcess::Pillar() {
  printf("Pillar\n");
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(channelingAnimation);
  mChanneling = ETrue;
  mStateTimer = 0;
  for (TInt n = 0; n < 6; n++) {
    TFloat angle = n * M_PI / 3;

    mGameState->AddProcess(new GWizardPillarProcess(mGameState, this, angle, 40, EFalse));
  }
}

void GWizardProcess::Teleport() {
  printf("Teleport\n");
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(teleportAnimation1);
  mStep = 0;
}

void GWizardProcess::Illusion() {
  printf("Illusion\n");
  mSprite->vx = mSprite->vy = 0;
  for (TInt i = 0; i < 8; i++) {
    RandomLocation();
    auto *p = new GWizardDecoyProcess(mGameState, this, mSprite->x, mSprite->y, mSpriteSheet);
    mGameState->AddProcess(p);
  }
  mSprite->StartAnimation(channelingAnimation);
  mChanneling = ETrue;
  mStateTimer = FRAMES_PER_SECOND * 3;
  StartBlink(0);
  mSprite->mFill = -1;
}

void GWizardProcess::Death() {
  mSprite->vx = mSprite->vy = 0;
  mChanneling = EFalse;
  mSprite->StartAnimation(deathAnimation);
  // get coordinates for explosion placement
  TRect r;
  mSprite->GetRect(r);
  //  r.Dump();
  mDeathCounter = 10;
  for (TInt delay = 0; delay < mDeathCounter; delay++) {
    printf("DEATH SPRITE @ %d,%d\n", r.x1, r.x2);
    auto *p = new GWizardDeathProcess(mGameState, this, r.x1, r.y1 - 48, delay);
    mGameState->AddProcess(p);
  }
}
// change wizard state
void GWizardProcess::SetState(TInt aState, DIRECTION aDirection) {
  mState = aState;
  mStep = 0;
  mDirection = aDirection;
  // set animation
  switch (mState) {
    case STATE_IDLE:
      Idle(mDirection);
      break;
    case STATE_WALK:
      Walk(mDirection);
      break;
    case STATE_PROJECTILE:
      Projectile();
      break;
    case STATE_PILLAR:
      Pillar();
      break;
    case STATE_TELEPORT:
      Teleport();
      break;
    case STATE_ILLUSION:
      Illusion();
      break;
    case STATE_DEATH:
      Death();
      break;
    default:
      Panic("GWizardProcess: invalid SetState(%d)\n", mState);
  }
}

TBool GWizardProcess::MaybeDamage() {
  if (mSprite->TestCType(STYPE_SPELL)) {
    mSprite->ClearCType(STYPE_SPELL);

    if (GPlayer::MaybeDamage(this, ETrue)) {
      mSprite->vx = mSprite->vy = 0;
      mSpellCounter += 2;
      auto *p = new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y + 1);
      mGameState->AddProcess(p);
      p = new GSpellOverlayProcess(mGameState, this, mSprite->x + 44, mSprite->y + 1);
      mGameState->AddProcess(p);
      StartBlink(FRAMES_PER_SECOND / 4);
      return ETrue;
    }
  }

  if (mSprite->TestCType(STYPE_PBULLET)) {
    mSprite->ClearCType(STYPE_PBULLET);
    if (GPlayer::MaybeDamage(this, EFalse)) {
      mSprite->Nudge(); // move sprite so it's not on top of player
      StartBlink(FRAMES_PER_SECOND / 4);
      return ETrue;
    }
  }

  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->Nudge();
  }

  return EFalse;
}

TBool GWizardProcess::MaybeAttack() {
  if (--mAttackTimer < 0) {
    printf("Attack!\n");
    SetState(Random() & 1u ? STATE_PROJECTILE : STATE_PILLAR, mDirection);
    return ETrue;
  }
  return EFalse;
}

TBool GWizardProcess::MaybeDeath() {
  if (mSprite->mHitPoints <= 0) {
    printf("WIZARD DEATH\n");
    mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
    SetState(STATE_DEATH, mDirection);
    return ETrue;
  }
  return EFalse;
}

// called each frame while wizard is idle
TBool GWizardProcess::IdleState() {
  if (mAttackTimer < 1.5 * FRAMES_PER_SECOND && ABS(mSprite->x - GPlayer::mSprite->mLastX) < 64 && ABS(mSprite->y - GPlayer::mSprite->mLastY) < 64) {
    SetState(STATE_TELEPORT, mDirection);
    return ETrue;
  }

  MaybeDamage();

  if (MaybeDeath()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  if (--mStateTimer < 1) {
    mStateTimer = 60;
    for (TInt i = 0; i < 10; i++) {
      DIRECTION d = (Random() & 1u) ? DIRECTION_RIGHT : DIRECTION_LEFT;
      if (mSprite->CanWalk(d == DIRECTION_RIGHT ? WALK_VELOCITY : -WALK_VELOCITY, 0)) {
        SetState(STATE_WALK, d);
        return ETrue;
      }
    }
    SetState(STATE_WALK, (Random() & 1u) ? DIRECTION_RIGHT : DIRECTION_LEFT);
  }
  return ETrue;
}

TBool GWizardProcess::WalkState() {
  MaybeDamage();

  if (MaybeDeath()) {
    return ETrue;
  }

  if (MaybeAttack()) {
    return ETrue;
  }

  // check to see if wizard has met a wall
  if (!mSprite->CanWalk(mSprite->vx, mSprite->vy)) {
    SetState(STATE_IDLE, DIRECTION_DOWN);
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    Walk(mDirection);
  }
  return ETrue;
}

TBool GWizardProcess::ProjectileState() {
  MaybeDamage();

  if (MaybeDeath()) {
    return ETrue;
  }

  if (mStep < 360) {
    if (mStateTimer-- <= 0) {
      // TODO: @jaygarcia SfxWizardCreateProjectile
      TFloat angle = 2 * M_PI;
      switch (mAttackType) {
        default:
        case 0:
          angle *= TFloat(mStep) / 360;
          mStep += 120;
          mStateTimer = FRAMES_PER_SECOND / 4;
          break;
        case 1:
          angle *= TFloat(mStep) / 360;
          mStep += 60;
          mStateTimer = FRAMES_PER_SECOND / 4;
          break;
        case 2:
          angle *= TFloat(mStep) / 360 - 720; // negative angle signals projectile should not be launched at player
          mStep += 40;
          mStateTimer = FRAMES_PER_SECOND / 8;
          break;
      }
      mGameState->AddProcess(new GWizardProjectileProcess(mGameState, this, angle, mType));
    }
    return ETrue;
  }

  if (mStep == 360) {
    mChanneling = EFalse;
    mSprite->StartAnimation(projectileAnimation2);
    mStep++;
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    SetAttackTimer();
    SetState(STATE_IDLE, mDirection);
    return ETrue;
  }

  return ETrue;
}

TBool GWizardProcess::PillarState() {
  if (MaybeDamage()) {
    // TODO: @jaygarcia SfxWizardSpellInterrupt
    mChanneling = EFalse;
    SetAttackTimer();
    SetState(STATE_IDLE, mDirection);
    return ETrue;
  }

  if (mStateTimer-- < 0) {
    // TODO: @jaygarcia SfxWizardCreatePillar

    // spawn pillars
    if (mType == ATTR_WIZARD_WATER || mType == ATTR_WIZARD_FIRE) {
      // Follows Player if water or fire
      mGameState->AddProcess(
              new GWizardPillarProcess(mGameState, this, 0, 0, ETrue, FRAMES_PER_SECOND * 6));
      mStateTimer = 0.3 * FRAMES_PER_SECOND;
    } else {
      for (TInt n = 0; n < 8; n++) {
        TFloat angle = RandomFloat() * 2 * M_PI,
                distance = Random(50, 100);

        mGameState->AddProcess(new GWizardPillarProcess(mGameState, this, angle, distance, EFalse, FRAMES_PER_SECOND * 4));
        mStateTimer = 2 * FRAMES_PER_SECOND;
      }

    }
  }

  return ETrue;
}

TBool GWizardProcess::TeleportState() {
  --mAttackTimer;
  if (mSprite->TestAndClearCType(STYPE_PBULLET)) {
    // TODO: @jaygarcia SfxWizardTeleport
    RandomLocation();
    mSprite->StartAnimation(teleportAnimation2);
    mStep++;
  }

  MaybeDamage();

  if (MaybeDeath()) {
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    if (mStep == 0) {
      // TODO: @jaygarcia SfxWizardTeleport
      RandomLocation();
      // illusion sometimes when below 75% health
      if (TFloat(mSprite->mHitPoints) / TFloat(mSprite->mMaxHitPoints) < 0.75 && (Random() & 1u)) {
        SetState(STATE_ILLUSION, mDirection);
        return ETrue;
      }
      mSprite->StartAnimation(teleportAnimation2);
      mStep++;
    } else {
      SetState(STATE_IDLE, mDirection);
      return ETrue;
    }
  }

  return ETrue;
}

TBool GWizardProcess::IllusionState() {
  if (MaybeDamage() || mSprite->mHitPoints == mSprite->mMaxHitPoints) {
    // TODO: @jaygarcia SfxWizardSpellInterrupt
    mChanneling = EFalse;
    SetState(STATE_IDLE, mDirection);
  }
  if (mStateTimer-- < 0) {
    // TODO: @jaygarcia SfxWizardHeal (or use existing SfxPlayerQuaffHealthPotion)
    mStateTimer = FRAMES_PER_SECOND * 3;
    auto *p = new GStatProcess(mSprite->x + 72, mSprite->y + 32, "%d", MIN(HEAL_RATE, mSprite->mMaxHitPoints - mSprite->mHitPoints));
    p->SetMessageType(STAT_HEAL);
    mSprite->mGameState->AddProcess(p);
    mSprite->mHitPoints = MIN(mSprite->mHitPoints + HEAL_RATE, mSprite->mMaxHitPoints);
  }
  return ETrue;
}

TBool GWizardProcess::DeathState() {
  if (mDeathCounter <= 3) {
    printf("drop $%x %d\n", mAttribute, mAttribute);
    GItemProcess::SpawnItem(mGameState, mIp, mAttribute, GPlayer::mSprite->x + 32, GPlayer::mSprite->y);
    return EFalse;
  }
  // maybe drop item
  return ETrue;
}

TBool GWizardProcess::RunBefore() {
  GLivingProcess::RunBefore();
  // Patch to prevent the damn thing from going out of screen.
  if ((mSprite->x < 0) || (mSprite->y <0)) {
    mSprite->x = mStartX;
    mSprite->y = mStartY;
  }

  switch (mState) {
    case STATE_IDLE:
      return IdleState();
    case STATE_WALK:
      return WalkState();
    case STATE_PROJECTILE:
      return ProjectileState();
    case STATE_PILLAR:
      return PillarState();
    case STATE_TELEPORT:
      return TeleportState();
    case STATE_ILLUSION:
      return IllusionState();
    case STATE_DEATH:
      return DeathState();
    default:
      Panic("GWizardProcess invalid mState %d\n", mState);
  }
  return ETrue;
}

TBool GWizardProcess::RunAfter() {
  GLivingProcess::RunAfter();
  if (mHitTimer-- < 0) {
    mHitTimer = HIT_SPAM_TIME;
  }
  return ETrue;
}

void GWizardProcess::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&mIp, sizeof(mIp));
  aStream.Write(&mState, sizeof(mState));
  aStream.Write(&mStep, sizeof(mStep));
  aStream.Write(&mAttackTimer, sizeof(mAttackTimer));
  aStream.Write(&mStateTimer, sizeof(mStateTimer));
  mSprite->WriteToStream(aStream);
}

void GWizardProcess::ReadFromStream(BMemoryStream &aStream) {
  aStream.Read(&mIp, sizeof(mIp));
  aStream.Read(&mState, sizeof(mState));
  aStream.Read(&mStep, sizeof(mStep));
  aStream.Read(&mAttackTimer, sizeof(mAttackTimer));
  aStream.Read(&mStateTimer, sizeof(mStateTimer));
  mSprite->ReadFromStream(aStream);
}
