#include "GMidBossGenericProcess.h"
#include "GMidBossProjectileProcess.h"
#include "GPlayer.h"

const TFloat VELOCITY = 2.0;
const TFloat BOUNCE_VELOCITY = 2.5;

const TInt16 IDLE_SPEED = 20;
const TInt16 HIT_SPEED = 5;
const TInt16 WALK_SPEED = 4;
const TInt16 TRANSFORM_SPEED = 8;
const TInt16 MOVE_SPEED = 4;

// region  ANIMATIONS {{{
static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 1),
  AEND,
};

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 3),
  ALOOP,
};

static ANIMSCRIPT hitAnimation[] = {
  ABITMAP(BOSS_SLOT),

  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_MID_BOSS_IDLE + 1),

  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_MID_BOSS_IDLE + 1),

  AFILL(COLOR_WHITE),
  ASTEP(HIT_SPEED, IMG_MID_BOSS_IDLE + 1),

  AFILL(-1),
  ASTEP(HIT_SPEED, IMG_MID_BOSS_IDLE + 1),

  ASTEP(HIT_SPEED * 4, IMG_MID_BOSS_IDLE + 3),
  ASTEP(HIT_SPEED * 4, IMG_MID_BOSS_IDLE + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 2),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT transformAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 3),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 7),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 11),
  AEND,
};

static ANIMSCRIPT moveAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(-32, 22),
  ASIZE(0, 16, 32, 24),
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
  ABITMAP(BOSS_SLOT),
  ASIZE(0, 8, 44, 24),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 11),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 7),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 3),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 1),
  AEND,
};

/* endregion }}} */

GMidBossGenericProcess::GMidBossGenericProcess(GGameState *aGameState, TFloat aX, TFloat aY, TUint16 aSlot, TInt aIp, TUint16 aAttribute, TUint16 aDropsItemAttribute, TInt16 aSpriteSheet)
    : GMidBossProcess(aGameState, aX, aY, aSlot, aIp, aAttribute, aDropsItemAttribute, aSpriteSheet) {
  //
  NewState(MB_IDLE_STATE, DIRECTION_DOWN);
}

GMidBossGenericProcess::~GMidBossGenericProcess() {
  //
}

void GMidBossGenericProcess::Idle(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossGenericProcess::Walk(DIRECTION aDirection) {
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
    default:
      Panic("GMidBossGenericProcess no Walk direction\n");
      break;
  }
  mStep = !mStep;
}

TBool GMidBossGenericProcess::MaybeAttack() {
  if (mState != MB_IDLE_STATE) {
    return EFalse;
  }
  if (--mAttackTimer > 0) {
    return EFalse;
  }
  mAttackTimer = MID_BOSS_ATTACK_TIME;
  GProcess *p = (GProcess *)new GMidBossProjectileProcess(mGameState, mSprite->x + 32, mSprite->y - 32);
  mGameState->AddProcess(p);
  return ETrue;
}

// boss changes into ball
void GMidBossGenericProcess::Ball(DIRECTION aDirection) {
  mSprite->StartAnimation(transformAnimation);
}

void GMidBossGenericProcess::Move(DIRECTION aDirection) {
  GAnchorSprite *player = GPlayer::mSprite;
  mSprite->StartAnimation(moveAnimation);
  mSprite->vx = mSprite->x > player->x ? -BOUNCE_VELOCITY : BOUNCE_VELOCITY;
  mSprite->vy = mSprite->y > player->y ? -BOUNCE_VELOCITY : BOUNCE_VELOCITY;
}

// ball returns to top of screen
void GMidBossGenericProcess::Return(DIRECTION aDirection) {
  mSprite->StartAnimation(moveAnimation);
}

// This is changing from ball back to normal state
void GMidBossGenericProcess::Revert(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0.;
  mSprite->StartAnimation(revertAnimation);
}

void GMidBossGenericProcess::Attack(DIRECTION aDirection) {
  mSprite->StartAnimation(idleAnimation);
}

void GMidBossGenericProcess::Hit(DIRECTION aDirection) {
  mSprite->vx = mSprite->vy = 0;
  mSprite->StartAnimation(hitAnimation);
}

void GMidBossGenericProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}

void GMidBossGenericProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitAnimation);
}
