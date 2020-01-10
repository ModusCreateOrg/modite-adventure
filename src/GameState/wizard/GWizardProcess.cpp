#include "GWizardProcess.h"
#include "GWizardProjectileProcess.h"
#include "GPlayer.h"

#define DEBUGME
//#undef DEBUGME

const TInt16 WALK_SPEED = 8;
const TFloat WALK_VELOCITY = 1.0;
const TInt16 ATTACK_SPEED = 5;

enum {
  STATE_IDLE,
  STATE_WALK,
  STATE_PROJECTILE,
  STATE_TELEPORT,
};

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(1, IMG_WIZARD_IDLE + 2),
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

static ANIMSCRIPT projectileAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  ASTEP(ATTACK_SPEED * 3, IMG_WIZARD_IDLE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_IDLE + 6),
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
  ASTEP(ATTACK_SPEED * 3, IMG_WIZARD_IDLE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_IDLE + 6),
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
  ANULL(100),
  AEND,
};

static ANIMSCRIPT teleportAnimation3[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 4),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 3),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 2),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_FIRE + 0),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_IDLE + 6),
  ASTEP(ATTACK_SPEED * 3, IMG_WIZARD_IDLE + 1),
  ASTEP(ATTACK_SPEED, IMG_WIZARD_WALK_DOWN + 0),
  AEND,
};

GWizardProcess::GWizardProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TInt aIp, TInt aType, TUint16 aAttribute, TUint16 aSpriteSheet)
    : GProcess(aAttribute) {
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
  //  mSprite->mSpriteSheet = gResourceManager.LoadSpriteSheet(aSpriteSheet);
  mSprite->type = STYPE_ENEMY;
  //  mSprite->SetCMask(STYPE_PLAYER | STYPE_PBULLET);
  mSprite->SetFlags(SFLAG_CHECK | SFLAG_RENDER_SHADOW);
  mSprite->SetFlags(SFLAG_CHECK);
  mSprite->x = aX;
  mSprite->y = aY;
  mSprite->cx = 10;
  mSprite->cy = 0;
  mSprite->w = 22;
  mSprite->h = 24;
  mStateTimer = 3 * 60;
  mStep = 0;
  SetState(STATE_IDLE, DIRECTION_DOWN);
  mGameState->AddSprite(mSprite);
  mAttackType = EFalse;
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
  mAttackTimer = Random(60, 180);
}

// start wizard idle in specified direction
void GWizardProcess::Idle(DIRECTION aDirection) {
  mDirection = aDirection;
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(idleAnimation);
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
    default:
      Panic("GWizardProcess: invalid SetState(%d)\n", mState);
  }
}

TBool GWizardProcess::MaybeHit() {
  return EFalse;
}

TBool GWizardProcess::MaybeAttack() {
  if (--mAttackTimer < 0) {
    printf("Attack! %s\n", mAttackType ? "TELEPORT" : "PROJECTILE");
    SetState(mAttackType ? STATE_TELEPORT : STATE_PROJECTILE, mDirection);
    mAttackType = !mAttackType;
    return ETrue;
  }
  return EFalse;
}

// called each frame while wizard is idle
TBool GWizardProcess::IdleState() {
  if (--mStateTimer < 1) {
    mStateTimer = 5 * 60;
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
    // return
  }
  if (MaybeAttack()) {
    // attack
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
    // fire 1-3 projectiled
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
  if (!mSprite->AnimDone()) {
    return ETrue;
  }
  printf("AnimDone(%d)\n", mStep);
  if (!mStep) {
    printf("ANIM2\n");
    mSprite->StartAnimation(teleportAnimation2);
    mSprite->ClearFlags(SFLAG_RENDER | SFLAG_CHECK);
    mStep++;
    mSprite->x = mStartX;
    mSprite->y = mStartY;
    mStateTimer = 100;
    return ETrue;
  }
  else if (mStep == 1 && --mStateTimer < 1) {
    printf("ANIM3\n");
    mState++;
    mSprite->StartAnimation(teleportAnimation3);
  }
  else {
    printf("ANIM4\n");
    mSprite->SetFlags(SFLAG_RENDER | SFLAG_CHECK);
    SetAttackTimer();
    SetState(STATE_IDLE, mDirection);
  }

  return ETrue;
}

TBool GWizardProcess::RunBefore() {
  switch (mState) {
    case STATE_IDLE:
      return IdleState();
    case STATE_WALK:
      return WalkState();
    case STATE_PROJECTILE:
      return ProjectileState();
    case STATE_TELEPORT:
      return TeleportState();
    default:
      Panic("GWizardProcess invalid mState %d\n", mState);
  }
  return ETrue;
}

TBool GWizardProcess::RunAfter() {
  mSprite->ClearCType(STYPE_PLAYER | STYPE_PBULLET);
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
