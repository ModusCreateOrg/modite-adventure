#include "GWizardProcess.h"
#include "GWizardProjectileProcess.h"
#include "GWizardPillarProcess.h"
#include "GWizardDeathProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"
#include "GItemProcess.h"

#define DEBUGME
//#undef DEBUGME

const TInt16 IDLE_SPEED = 8;
const TInt16 WALK_SPEED = 8;
const TFloat WALK_VELOCITY = 1.25;
const TInt16 ATTACK_SPEED = 5;
const TInt16 HIT_SPEED = 5;
const TInt HIT_SPAM_TIME = 2 * FRAMES_PER_SECOND;

enum {
  STATE_IDLE,
  STATE_WALK,
  STATE_PROJECTILE,
  STATE_TELEPORT,
  STATE_HIT,
  STATE_SPELL,
  STATE_DEATH,
};

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(1, IMG_WIZARD_IDLE + 3),
  AEND,
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

static ANIMSCRIPT hitAnimation[] = {
  ABITMAP(BOSS_SLOT),

  AFILL(COLOR_TEXT),
  ASTEP(HIT_SPEED, IMG_WIZARD_IDLE + 1),

  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_WIZARD_IDLE + 1),

  AFILL(COLOR_TEXT),
  ASTEP(HIT_SPEED, IMG_WIZARD_IDLE + 1),

  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_WIZARD_IDLE + 1),

  ASTEP(HIT_SPEED * 4, IMG_WIZARD_IDLE + 3),
  ASTEP(HIT_SPEED * 4, IMG_WIZARD_IDLE + 1),
  AEND,
};

static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(IDLE_SPEED, IMG_WIZARD_IDLE + 1),
  AEND,
};

static ANIMSCRIPT projectileAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 7),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 4),
  AEND,
};

static ANIMSCRIPT projectileAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_IDLE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_IDLE + 1),
  AEND,
};

static ANIMSCRIPT teleportAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  ASTEP(ATTACK_SPEED * 4, IMG_WIZARD_WALK_DOWN + 7),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 4),
  ANULL(1),
  AEND,
};

static ANIMSCRIPT teleportAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ANULL(300),
  AEND,
};

static ANIMSCRIPT teleportAnimation3[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 4),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED * 6, IMG_WIZARD_IDLE + 6),
  ASTEP(ATTACK_SPEED * 6, IMG_WIZARD_IDLE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  AEND,
};

// constructor
GWizardProcess::GWizardProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TInt aIp, TInt aType, TUint16 aAttribute, TUint16 aSpriteSheet)
    : GProcess(aAttribute) {
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
  mSprite->Name("WIZARD");
  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(aSpriteSheet);
  mSprite->type = STYPE_ENEMY;
  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT);
//  mSprite->SetFlags(SFLAG_RENDER_SHADOW | SFLAG_KNOCKBACK | SFLAG_CHECK);
  mSprite->SetFlags(SFLAG_KNOCKBACK | SFLAG_CHECK);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->cx = 2;
  mSprite->cy = 0;
  mSprite->w = 30;
  mSprite->h = 24;
  mSprite->SetStatMultipliers(4.0, 1.2, 10.0);
  mGameState->AddSprite(mSprite);
  //
  mHitTimer = HIT_SPAM_TIME;
  mStateTimer = 2 * 60;
  mStep = 0;
  mDeathCounter = 0;
  mSpellCounter = 0;
  mAttackType = EFalse;
  SetState(STATE_IDLE, DIRECTION_DOWN);
  SetAttackTimer();
}

GWizardProcess::~GWizardProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

void GWizardProcess::SetAttackTimer() {
//  mAttackTimer = 5;
  mAttackTimer = Random(1, 30);
}

// start wizard idle in specified direction
void GWizardProcess::Idle(DIRECTION aDirection) {
  mDirection = aDirection;
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(idleAnimation);
  mSprite->mInvulnerable = EFalse;
}

// start wizard walking in specified direction
void GWizardProcess::Walk(DIRECTION aDirection) {
  mDirection = aDirection;
  mStep = 1 - mStep;
  switch (mDirection) {
    case DIRECTION_UP:
      mSprite->vx = 0;
      mSprite->vy = -WALK_VELOCITY;
      mSprite->StartAnimation(mStep ? walkUpAnimation2 : walkUpAnimation1);
      break;
    case DIRECTION_DOWN:
      mSprite->vx = 0;
      mSprite->vy = WALK_VELOCITY;
      mSprite->StartAnimation(mStep ? walkDownAnimation2 : walkDownAnimation1);
      break;
    case DIRECTION_LEFT:
      mSprite->vx = -WALK_VELOCITY;
      mSprite->vy = 0;
      mSprite->StartAnimation(mStep ? walkLeftAnimation2 : walkLeftAnimation1);
      break;
    case DIRECTION_RIGHT:
      mSprite->vx = WALK_VELOCITY;
      mSprite->vy = 0;
      mSprite->StartAnimation(mStep ? walkRightAnimation2 : walkRightAnimation1);
      break;
    default:
      break;
  }
}

void GWizardProcess::Projectile(DIRECTION aDirection) {
  printf("Projectile\n");
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(projectileAnimation1);
  mStep = 0;
}

void GWizardProcess::Teleport(DIRECTION aDirection) {
  printf("Teleport\n");
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(teleportAnimation1);
  mStep = 0;
  mSprite->mInvulnerable = ETrue;
}

void GWizardProcess::Hit(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(hitAnimation);
}

void GWizardProcess::Spell(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(hitAnimation);
  mSpellCounter += 2;
  auto *p = new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y + 1);
  //  mSpellOverlayProcess = p;
  mGameState->AddProcess(p);
  p = new GSpellOverlayProcess(mGameState, this, mSprite->x + 44, mSprite->y + 1);
  mGameState->AddProcess(p);
}

void GWizardProcess::Death(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
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
      Projectile(mDirection);
      break;
    case STATE_TELEPORT:
      Teleport(mDirection);
      break;
    case STATE_HIT:
      Hit(mDirection);
      break;
    case STATE_DEATH:
      Death(mDirection);
      break;
    default:
      Panic("GWizardProcess: invalid SetState(%d)\n", mState);
  }
}

TBool GWizardProcess::MaybeHit() {
  if (mSprite->TestCType(STYPE_SPELL)) {
    mSprite->ClearCType(STYPE_SPELL);

    if (!mSprite->mInvulnerable) {
      mSprite->mInvulnerable = ETrue;
      // TODO take into account which spellbook is being wielded
      // random variation from 100% to 150% base damage
      TInt hitAmount = GPlayer::mHitStrength + round(RandomFloat() * GPlayer::mHitStrength / 2);
      mSprite->mHitPoints -= hitAmount;

      auto *p = new GStatProcess(mSprite->x + 80, mSprite->y + 32, "%d", hitAmount);
      p->SetMessageType(STAT_ENEMY_HIT);
      mGameState->AddProcess(p);
      if (mSprite->mHitPoints <= 0) {
        printf("WIZARD  DEATH\n");
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
      }
      SetState(STATE_SPELL, mSprite->mDirection);
      return ETrue;
    }
  }

  GAnchorSprite *other = mSprite->mCollided;
  if (mSprite->TestCType(STYPE_PBULLET)) {
    mSprite->ClearCType(STYPE_PBULLET);
    if (!mSprite->mInvulnerable) {
      mSprite->Nudge(); // move sprite so it's not on top of player
      mSprite->mInvulnerable = ETrue;
      // random variation from 100% to 150% base damage
      TInt hitAmount = GPlayer::mHitStrength + round(RandomFloat() * GPlayer::mHitStrength / 2);
      mSprite->mHitPoints -= hitAmount;
      auto *p = new GStatProcess(mSprite->x + 80, mSprite->y + 32, "%d", hitAmount);
      p->SetMessageType(STAT_ENEMY_HIT);
      mGameState->AddProcess(p);
      if (mSprite->mHitPoints <= 0) {
        mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
      }
      switch (other->mDirection) {
        case DIRECTION_RIGHT:
          SetState(STATE_HIT, DIRECTION_LEFT);
          break;
        case DIRECTION_LEFT:
          SetState(STATE_HIT, DIRECTION_RIGHT);
          break;
        case DIRECTION_UP:
          SetState(STATE_HIT, DIRECTION_DOWN);
          break;
        case DIRECTION_DOWN:
          SetState(STATE_HIT, DIRECTION_UP);
          break;
        default:
          Panic("GWizardProcess no hit direction\n");
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

TBool GWizardProcess::MaybeAttack() {
  if (--mAttackTimer < 0) {
    printf("Attack! %s\n", mAttackType ? "TELEPORT" : "PROJECTILE");
//    SetState(STATE_TELEPORT, mDirection);
    SetState(mAttackType ? STATE_TELEPORT : STATE_PROJECTILE, mDirection);
    mAttackType = !mAttackType;
    return ETrue;
  }
  return EFalse;
}

// called each frame while wizard is idle
TBool GWizardProcess::IdleState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (--mStateTimer < 1) {
    mStateTimer = 60;
    for (TInt i = 0; i < 10; i++) {
      DIRECTION d = (Random() & 1) ? DIRECTION_RIGHT : DIRECTION_LEFT;
      if (mSprite->CanWalk(d, d == DIRECTION_RIGHT ? WALK_VELOCITY : -WALK_VELOCITY, 0)) {
        SetState(STATE_WALK, d);
        return ETrue;
      }
    }
    SetState(STATE_WALK, (Random() & 1) ? DIRECTION_RIGHT : DIRECTION_LEFT);
  }
  return ETrue;
}

TBool GWizardProcess::WalkState() {
  if (MaybeHit()) {
    return ETrue;
  }
  if (MaybeAttack()) {
    return ETrue;
  }

  // check to see if wizard has met a wall
  if (!mSprite->CanWalk(mDirection, mSprite->vx, mSprite->vy)) {
    SetState(STATE_IDLE, DIRECTION_DOWN);
    return ETrue;
  }

  if (mSprite->AnimDone()) {
    Walk(mDirection);
  }
  return ETrue;
}

TBool GWizardProcess::ProjectileState() {
  if (!mSprite->AnimDone()) {
    return ETrue;
  }
  if (!mStep) {
    // fire 1-3 projectiles
    printf("FIRE!\n");
    mGameState->AddProcess(new GWizardProjectileProcess(mGameState, this, 0));
    mGameState->AddProcess(new GWizardProjectileProcess(mGameState, this, 1));
    mGameState->AddProcess(new GWizardProjectileProcess(mGameState, this, 2));
    mSprite->StartAnimation(projectileAnimation2);
    mStep++;
    return ETrue;
  }
  SetAttackTimer();
  SetState(STATE_IDLE, mDirection);
  return ETrue;
}

TBool GWizardProcess::TeleportState() {


  // check to see if wizard has met a wall
  TBool canWalkUp = mSprite->CanWalk(DIRECTION_UP, mSprite->vx, mSprite->vy),
        canWalkLt = mSprite->CanWalk(DIRECTION_LEFT, mSprite->vx, mSprite->vy),
        canWalkRt = mSprite->CanWalk(DIRECTION_RIGHT, mSprite->vx, mSprite->vy),
        canWalkDn = mSprite->CanWalk(DIRECTION_DOWN, mSprite->vx, mSprite->vy);


  // Bound around the room while the player is being attacked by the spikes/pillars
  if (! canWalkUp) {
    mSprite->vy *= -1;
  }
  if (! canWalkDn) {
    mSprite->vy *= -1;
  }

  if (! canWalkLt) {
    mSprite->vx *= -1;
  }
  if (! canWalkRt) {
    mSprite->vx *= -1;
  }


//
//  //  SetState(STATE_IDLE, DIRECTION_DOWN);
//  //  return ETrue;
//  }
  if (!mSprite->AnimDone()) {
    return ETrue;
  }


  if (!mStep) {
    mSprite->StartAnimation(teleportAnimation2);
    mSprite->ClearFlags(SFLAG_RENDER | SFLAG_CHECK); // Let's not hit the player.
    mStep++;

//    int nTries = 0;
    while (mSprite->vx == 0) {
//      nTries ++;
      mSprite->vx = (TFloat)Random(-3, 3);
    }
//    printf("nTries VX = %i\n", nTries);

//    nTries = 0;
    while (mSprite->vy == 0) {
//      nTries++;
      mSprite->vy = (TFloat)Random(-3, 3);
    }
//    printf("nTries VX = %i\n", nTries);


    // spawn pillars
    for (TInt n = 0; n < 8; n++) {
      // Follows Player if water or fire
      if (mType == ATTR_WIZARD_WATER || mType == ATTR_WIZARD_FIRE) {
        mGameState->AddProcess(new GWizardPillarProcess(mGameState, GPlayer::mSprite->mLastX, GPlayer::mSprite->mLastY, ETrue, (n * 30)));
      }
      else {
        TInt pillarX = mSprite->x + Random(-64, 64),
             pillarY = mSprite->y + Random(-64, 64);

        mGameState->AddProcess(new GWizardPillarProcess(mGameState, pillarX, pillarY, EFalse, 0));
      }

    }

    return ETrue;
  }
  else if (mStep == 1 && --mStateTimer < 1) {
    mState++;
    mSprite->StartAnimation(teleportAnimation3);
  }
  else {
    mSprite->SetFlags(SFLAG_RENDER | SFLAG_CHECK);
    SetAttackTimer();
    SetState(STATE_IDLE, mDirection);
  }

  return ETrue;
}

TBool GWizardProcess::HitState() {
#ifdef __CHICKEN_MODE__
  mSprite->mHitPoints = 0;
#endif

  if (mSprite->TestCType(STYPE_PLAYER)) {
    mSprite->ClearCType(STYPE_PLAYER);
    mSprite->Nudge();
  }

  if (mHitTimer < 0) {
    mHitTimer = HIT_SPAM_TIME;
    mSprite->mInvulnerable = EFalse;
    mSprite->ClearCType(STYPE_PBULLET);
    SetState(STATE_TELEPORT, DIRECTION_DOWN);
  }

  if (mSprite->AnimDone()) {
    if (mSprite->mHitPoints <= 0) {
      SetState(STATE_DEATH, mSprite->mDirection);
    }
    else {
      mSprite->mInvulnerable = EFalse;
      mSprite->ClearCType(STYPE_PBULLET);
      SetState(STATE_IDLE, mSprite->mDirection);
    }
  }

  return ETrue;
}

TBool GWizardProcess::SpellState() {
  if (mSprite->AnimDone() && mSpellCounter <= 0) {
    if (mSprite->mHitPoints <= 0) {
      SetState(STATE_DEATH, mSprite->mDirection);
    }
    else {
      mSprite->mInvulnerable = EFalse;
      mSprite->ClearCType(STYPE_PBULLET);
      SetState(STATE_IDLE, mSprite->mDirection);
    }
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
    case STATE_TELEPORT:
      return TeleportState();
    case STATE_HIT:
      return HitState();
    case STATE_SPELL:
      return SpellState();
    case STATE_DEATH:
      return DeathState();
    default:
      Panic("GWizardProcess invalid mState %d\n", mState);
  }
  return ETrue;
}

TBool GWizardProcess::RunAfter() {
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
