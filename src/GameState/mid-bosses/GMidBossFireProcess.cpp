#include "GMidBossFireProcess.h"
#include "GMidBossFireballProcess.h"
#include "GPlayer.h"

const TFloat VELOCITY = 2.0;
const TFloat BOUNCE_VELOCITY = 2.5;

const TInt16 IDLE_SPEED = 20;
const TInt16 HIT_SPEED = 20;
const TInt16 WALK_SPEED = 4;
const TInt16 TRANSFORM_SPEED = 8;
const TInt16 MOVE_SPEED = 4;

// region  ANIMATIONS {{{
static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 1),
  AEND,
};

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(MID_BOSS_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 3),
  ALOOP,
};

static ANIMSCRIPT hitAnimation[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(HIT_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(HIT_SPEED, IMG_MID_BOSS_IDLE + 3),
  ASTEP(HIT_SPEED, IMG_MID_BOSS_IDLE + 1),
  AEND,
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

static ANIMSCRIPT transformAnimation[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 3),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 7),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 11),
  AEND,
};

static ANIMSCRIPT moveAnimation[] = {
  ABITMAP(MID_BOSS_SLOT),
  ADELTA(-32, 22),
  ASIZE(0, 0, 32, 40),
  ALABEL,
  ASTEP(MOVE_SPEED, IMG_MID_BOSS_MOVE + 0),
  ASTEP(MOVE_SPEED, IMG_MID_BOSS_MOVE + 1),
  ASTEP(MOVE_SPEED, IMG_MID_BOSS_MOVE + 2),
  ASTEP(MOVE_SPEED, IMG_MID_BOSS_MOVE + 3),
  ASTEP(MOVE_SPEED, IMG_MID_BOSS_MOVE + 2),
  ASTEP(MOVE_SPEED, IMG_MID_BOSS_MOVE + 1),
  ALOOP,
};

static ANIMSCRIPT revertAnimation[] = {
  ABITMAP(MID_BOSS_SLOT),
  ASIZE(0, 0, 44, 75),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 11),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 7),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 3),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 1),
  AEND,
};

/* endregion }}} */

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
  BProcess *p = new GMidBossFireballProcess(mGameState, mSprite->x + 32, mSprite->y - 32);
  mGameState->AddProcess(p);
  return ETrue;
}

// boss changes into ball
void GMidBossFireProcess::Ball(DIRECTION aDirection) {
  mSprite->StartAnimation(transformAnimation);
}

void GMidBossFireProcess::Move(DIRECTION aDirection) {
  GAnchorSprite *player = GPlayer::mSprite;
  mSprite->StartAnimation(moveAnimation);
  mSprite->vx = mSprite->x > player->x ? -BOUNCE_VELOCITY : BOUNCE_VELOCITY;
  mSprite->vy = mSprite->y > player->y ? -BOUNCE_VELOCITY : BOUNCE_VELOCITY;
}

// ball returns to top of screen
void GMidBossFireProcess::Return(DIRECTION aDirection) {
  mSprite->StartAnimation(moveAnimation);
}

// This is changing from ball back to normal state
void GMidBossFireProcess::Revert(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0.;
  mSprite->StartAnimation(revertAnimation);
}

void GMidBossFireProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossFireProcess::Hit(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(hitAnimation);
}

void GMidBossFireProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}

void GMidBossFireProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitAnimation);
}
