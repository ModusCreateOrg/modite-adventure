#include "GMidBossFireProcess.h"
#include "GMidBossFireballProcess.h"

const TFloat VELOCITY = 2.0;

const TInt16 IDLE_SPEED = 20;
const TInt16 WALK_SPEED = 4;

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(MID_BOSS_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 3),
  ALOOP,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(MID_BOSS_SLOT),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(MID_BOSS_SLOT),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 2),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  AEND,
};

GMidBossFireProcess::GMidBossFireProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot)
    : GMidBossProcess(aGameState, aX, aY, aSlot) {
  //
  NewState(MB_IDLE_STATE, DIRECTION_DOWN);
}

GMidBossFireProcess::~GMidBossFireProcess() {
  //
}

void GMidBossFireProcess::Idle(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Walk(DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      mSprite->vy = -VELOCITY;
      mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
      break;
    case DIRECTION_DOWN:
      mSprite->vy = VELOCITY;
      mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
      break;
    case DIRECTION_LEFT:
      mSprite->vx = -VELOCITY;
      mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
      break;
    case DIRECTION_RIGHT:
      mSprite->vx = VELOCITY;
      mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
      break;
  }
  mStep = !mStep;
}

TBool GMidBossFireProcess::MaybeAttack() {
  if (mState != MB_IDLE_STATE) {
    return EFalse;
  }
  if (--mAttackTimer > 0) {
    return EFalse;
  }
  mAttackTimer = MID_BOSS_ATTACK_TIME;
  BProcess *p = new GMidBossFireballProcess(mGameState, mSprite->x, mSprite->y);
  mGameState->AddProcess(p);
  return ETrue;
}

void GMidBossFireProcess::Ball(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Return(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Revert(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Hit(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

