#include <math.h>
#include "GWizardProcess.h"
#include "GWizardProjectileProcess.h"
#include "GWizardPillarProcess.h"
#include "GWizardDeathProcess.h"
#include "GStatProcess.h"
#include "GPlayer.h"
#include "GItemProcess.h"
#include "GWizardDecoyProcess.h"

#define DEBUGME
//#undef DEBUGME

const TInt16 IDLE_SPEED = 4 * FACTOR;
const TInt16 WALK_SPEED = 4 * FACTOR;
const TFloat WALK_VELOCITY = 1.25;
const TInt16 ATTACK_SPEED = 2 * FACTOR;
const TInt HIT_SPAM_TIME = 2 * FRAMES_PER_SECOND;

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(1, IMG_WIZARD_IDLE + 3),
  AEND,
};

static ANIMSCRIPT channelingAnimation[] = {
  ALABEL,
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 4),
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
  ASTEP(IDLE_SPEED, IMG_WIZARD_IDLE + 1),
  AEND,
};

static ANIMSCRIPT projectileAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE_START),
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
  ASTEP(ATTACK_SPEED * 4, IMG_WIZARD_FIRE_START),
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
  AEND,
};

static ANIMSCRIPT teleportAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(300, IMG_WIZARD_WALK_DOWN),
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
  mSpriteSheet = aSpriteSheet;
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
  mAttackType = 2;
  SetState(STATE_IDLE, DIRECTION_DOWN);
  SetAttackTimer();
  GPlayer::mActiveBoss = mSprite;
}

GWizardProcess::~GWizardProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
  GPlayer::mActiveBoss = ENull;
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

void GWizardProcess::Projectile() {
  printf("Projectile\n");
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(projectileAnimation1);
  mStep = 0;
}

void GWizardProcess::Teleport() {
  printf("Teleport\n");
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(teleportAnimation1);
  mStep = 0;
}

void GWizardProcess::Illusion() {
  mSprite->vx = mSprite->vy = 0;
  TFloat theta0 = M_PI * 4 * RandomFloat(), theta, dx, dy;
  mSprite->x = mStartX;
  mSprite->y = mStartY;
  for (TInt i = 0; i < 8; i++) {
    theta = theta0 + i * M_PI_4;
    dx = 64 * SIN(theta);
    dy = 64 * COS(theta);
    if (i) {
      auto *p = new GWizardDecoyProcess(mGameState, this, mStartX + dx, mStartY + dy, mSpriteSheet);
      mGameState->AddProcess(p);
    } else {
      mSprite->x = mStartX + dx;
      mSprite->y = mStartY + dy;
    }

  }
  mSprite->StartAnimation(idleAnimation);
}

void GWizardProcess::Death() {
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
      Projectile();
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
  if (mBlinkTimer-- > 0) {
    mSprite->mFill = mBlinkTimer % 2 ? COLOR_WHITE : -1;
  }
  if (mSprite->TestCType(STYPE_SPELL)) {
    mSprite->ClearCType(STYPE_SPELL);

    if (GPlayer::MaybeDamage(mSprite, ETrue)) {
      mSprite->vx = mSprite->vy = 0;
      mSpellCounter += 2;
      auto *p = new GSpellOverlayProcess(mGameState, this, mSprite->x, mSprite->y + 1);
      mGameState->AddProcess(p);
      p = new GSpellOverlayProcess(mGameState, this, mSprite->x + 44, mSprite->y + 1);
      mGameState->AddProcess(p);
      mBlinkTimer = FRAMES_PER_SECOND / 4;
      return ETrue;
    }
  }

  if (mSprite->TestCType(STYPE_PBULLET)) {
    mSprite->ClearCType(STYPE_PBULLET);
    if (GPlayer::MaybeDamage(mSprite, EFalse)) {
      mSprite->Nudge(); // move sprite so it's not on top of player
      mBlinkTimer = FRAMES_PER_SECOND / 4;
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
    switch (mAttackType) {
      case 0:
        printf("Attack! %s\n", "TELEPORT");
        SetState(STATE_TELEPORT, mDirection);
        break;
      case 1:
        printf("Attack! %s\n", "PROJECTILE");
        SetState(STATE_PROJECTILE, mDirection);
        break;
      default:
        printf("Attack! %s\n", "ILLUSION");
        SetState(STATE_ILLUSION, mDirection);
        break;
    }
    mAttackType = 0;
    return ETrue;
  }
  return EFalse;
}

TBool GWizardProcess::MaybeDeath() {
  if (mSprite->mHitPoints <= 0) {
    printf("WIZARD DEATH\n");
    mGameState->AddProcess(new GStatProcess(mSprite->x + 72, mSprite->y, "EXP +%d", mSprite->mLevel));
    SetState(STATE_DEATH, mSprite->mDirection);
    return ETrue;
  }
  return EFalse;
}

// called each frame while wizard is idle
TBool GWizardProcess::IdleState() {
  MaybeDamage();

  if (MaybeDeath()) {
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
  MaybeDamage();

  if (MaybeDeath()) {
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
  MaybeDamage();

  if (MaybeDeath()) {
    return ETrue;
  }

  if (!mSprite->AnimDone()) {
    return ETrue;
  }
  if (!mStep) {
    // fire 1-3 projectiled
    TFloat xx = mSprite->x + 16,
           yy = mSprite->y - 16;

    // Angles are in radians
    const TFloat angleToPlayer = atan2(GPlayer::mSprite->y - yy, GPlayer::mSprite->x - xx);
    const TFloat step = 45. * (M_PI/180);
    const TFloat angles[3] = { angleToPlayer, angleToPlayer + step, angleToPlayer - step };

    mGameState->AddProcess(new GWizardProjectileProcess(mGameState, xx, yy, angles[0], mType));
    mGameState->AddProcess(new GWizardProjectileProcess(mGameState, xx, yy, angles[1], mType));
    mGameState->AddProcess(new GWizardProjectileProcess(mGameState, xx, yy, angles[2], mType));


    mSprite->StartAnimation(projectileAnimation2);
    mStep++;
    return ETrue;
  }
  SetAttackTimer();
  SetState(STATE_IDLE, mDirection);
  return ETrue;
}

TBool GWizardProcess::TeleportState() {
  MaybeDamage();

  if (MaybeDeath()) {
    return ETrue;
  }

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

TBool GWizardProcess::IllusionState() {
  if (MaybeDamage()) {
    SetState(STATE_IDLE, mDirection);
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
