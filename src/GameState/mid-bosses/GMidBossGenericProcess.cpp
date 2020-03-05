#include "GMidBossGenericProcess.h"
#include "GMidBossProjectileProcess.h"
#include "GPlayer.h"

const TFloat BOUNCE_VELOCITY = 2.5;

const TInt16 IDLE_SPEED = 10 * FACTOR;
const TInt16 HIT_SPEED = 2 * FACTOR;
const TInt16 WALK_SPEED = 2 * FACTOR;
const TInt16 TRANSFORM_SPEED = 4 * FACTOR;
const TInt16 MOVE_SPEED = 2 * FACTOR;

// region  ANIMATIONS {{{
static ANIMSCRIPT deathAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 1),
  AEND,
};

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_MID_BOSS_IDLE + 3),
  ALOOP,
};

static ANIMSCRIPT hitAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),

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
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT landDownAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_DOWN + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED * 5, IMG_MID_BOSS_WALK_DOWN + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT landRightAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_RIGHT + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED * 5, IMG_MID_BOSS_WALK_RIGHT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(4, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 0),
  ADELTA(2, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 2),
  ADELTA(2, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT landLeftAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(2, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(2, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(2, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(2, 0),
  ASTEP(WALK_SPEED * 5, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftWaterAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftWaterAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 2),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT landLeftWaterAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED * 5, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftFireAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(-6, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 0),
  ADELTA(-4, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkLeftFireAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(-2, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 2),
  ADELTA(-4, 0),
  AFLIP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT landLeftFireAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(-4, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(-4, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(-4, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_LEFT + 1),
  ADELTA(-4, 0),
  ASTEP(WALK_SPEED * 5, IMG_MID_BOSS_WALK_LEFT + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT landUpAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  ADELTA(0, 2),
  ASTEP(WALK_SPEED, IMG_MID_BOSS_WALK_UP + 1),
  ADELTA(0, 0),
  ASTEP(WALK_SPEED * 5, IMG_MID_BOSS_WALK_UP + 1),
  AEND,
};

static ANIMSCRIPT transformAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 0),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(TRANSFORM_SPEED * 2, IMG_MID_BOSS_IDLE + 3),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 1),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 3),
  ADELTA(0, 8),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 7),
  ADELTA(0, 16),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 11),
  AEND,
};

static ANIMSCRIPT moveAnimation[] = {
  ABITMAP(BOSS_SLOT),
  ADELTA(0, 16),
  ASIZE(32, 8, 32, 24),
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
  ADELTA(0, 0),
  ASIZE(20, 8, 44, 24),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 11),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 7),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 3),
  ASTEP(TRANSFORM_SPEED, IMG_MID_BOSS_IDLE + 1),
  AEND,
};

static ANIMSCRIPT* walkAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftAnimation2, walkRightAnimation2};
static ANIMSCRIPT* landAnimations[] = {landUpAnimation, landDownAnimation, landLeftAnimation, landRightAnimation};
// flipped sprites need different alignment depending on spritesheet
static ANIMSCRIPT* walkWaterAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftWaterAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkWaterAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftWaterAnimation2, walkRightAnimation2};
static ANIMSCRIPT* landWaterAnimations[] = {landUpAnimation, landDownAnimation, landLeftWaterAnimation, landRightAnimation};
static ANIMSCRIPT* walkFireAnimations1[] = {walkUpAnimation1, walkDownAnimation1, walkLeftFireAnimation1, walkRightAnimation1};
static ANIMSCRIPT* walkFireAnimations2[] = {walkUpAnimation2, walkDownAnimation2, walkLeftFireAnimation2, walkRightAnimation2};
static ANIMSCRIPT* landFireAnimations[] = {landUpAnimation, landDownAnimation, landLeftFireAnimation, landRightAnimation};

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
  mSprite->MoveInDirection(VELOCITY, aDirection);
  Charge(aDirection);
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

void GMidBossGenericProcess::Charge(DIRECTION aDirection) {
  switch (mAttribute) {
    case ATTR_MID_BOSS_WATER:
      mSprite->StartAnimationInDirection(mStep ? walkWaterAnimations1 : walkWaterAnimations2, aDirection);
      break;
    case ATTR_MID_BOSS_FIRE:
      mSprite->StartAnimationInDirection(mStep ? walkFireAnimations1 : walkFireAnimations2, aDirection);
      break;
    case ATTR_MID_BOSS_EARTH:
    case ATTR_MID_BOSS_ENERGY:
    default:
      mSprite->StartAnimationInDirection(mStep ? walkAnimations1 : walkAnimations2, aDirection);
      break;
  }
  mStep = !mStep;
}

void GMidBossGenericProcess::Land(DIRECTION aDirection) {
  switch (mAttribute) {
    case ATTR_MID_BOSS_WATER:
      mSprite->StartAnimationInDirection(landWaterAnimations, aDirection);
      break;
    case ATTR_MID_BOSS_FIRE:
      mSprite->StartAnimationInDirection(landFireAnimations, aDirection);
      break;
    case ATTR_MID_BOSS_EARTH:
    case ATTR_MID_BOSS_ENERGY:
    default:
      mSprite->StartAnimationInDirection(landAnimations, aDirection);
      break;
  }
}

void GMidBossGenericProcess::Death(DIRECTION aDirection) {
  mSprite->StartAnimation(deathAnimation);
}

void GMidBossGenericProcess::Spell(DIRECTION aDirection) {
  mSprite->StartAnimation(hitAnimation);
}
