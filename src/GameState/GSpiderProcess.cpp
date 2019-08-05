#include "GSpiderProcess.h"

const TInt IDLE_SPEED = 5;
static ANIMSCRIPT idleUpAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 2),
  ALOOP
};
static ANIMSCRIPT idleDownAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 2),
  ALOOP
};
static ANIMSCRIPT idleLeftAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 2),
  ALOOP
};
static ANIMSCRIPT idleRightAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 0),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 1),
  ASTEP(IDLE_SPEED, IMG_SPIDER_IDLE + 2),
  ALOOP
};

const TInt SELECT_SPEED = 5;
static ANIMSCRIPT selectAnimation[] = {
  ABITMAP(SPIDER_SLOT),
  ALABEL,
  ASTEP(SELECT_SPEED, IMG_SPIDER_SELECTED + 0),
  ASTEP(SELECT_SPEED, IMG_SPIDER_SELECTED + 1),
  ASTEP(SELECT_SPEED, IMG_SPIDER_SELECTED + 2),
  ALOOP
};

const TInt WALK_SPEED = 5;
const TFloat VELOCITY = 2;

static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_DOWN + 0),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_DOWN + 1),
  AEND
};
static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_DOWN + 2),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_DOWN + 3),
  AEND
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_UP + 0),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_UP + 1),
  AEND
};
static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_UP + 2),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_UP + 3),
  AEND
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_RIGHT + 1),
  AEND
};
static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_RIGHT + 2),
  ASTEP(SELECT_SPEED, IMG_SPIDER_WALK_RIGHT + 3),
  AEND
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(SPIDER_SLOT),
  AFLIP(SELECT_SPEED, IMG_SPIDER_WALK_RIGHT + 0),
  AFLIP(SELECT_SPEED, IMG_SPIDER_WALK_RIGHT + 1),
  AEND
};
static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(SPIDER_SLOT),
  AFLIP(SELECT_SPEED, IMG_SPIDER_WALK_RIGHT + 2),
  AFLIP(SELECT_SPEED, IMG_SPIDER_WALK_RIGHT + 3),
  AEND
};

GSpiderProcess::GSpiderProcess(GGameState *aGameState, GGamePlayfield *aGamePlayfield)
  : GEnemyProcess(aGameState,
                  aGamePlayfield,
                  SPIDER_SLOT,
                  SPIDER_PALETTE,
                  SPIDER_COLORS) {
  mSprite->x = 150.0;
  mSprite->y = 150.0;
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GSpiderProcess::~GSpiderProcess() {
  //
}

void GSpiderProcess::NewState(TUint16 aState, TUint16 aDirection) {
  mState = aState;
  mDirection = aDirection;
  mSprite->mDx = 0;
  mSprite->mDy = 0;
  switch (aState) {
    case IDLE_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      switch (mDirection) {
        case DIRECTION_UP:
          mSprite->StartAnimation(idleUpAnimation);
          break;
        case DIRECTION_DOWN:
          mSprite->StartAnimation(idleDownAnimation);
          break;
        case DIRECTION_LEFT:
          mSprite->StartAnimation(idleLeftAnimation);
          break;
        case DIRECTION_RIGHT:
          mSprite->StartAnimation(idleRightAnimation);
          break;
      }
      break;
    case WALK_STATE:
      mSprite->vx = 0;
      mSprite->vy = 0;
      switch (mDirection) {
        case DIRECTION_UP:
          mStep = 1 - mStep;
          mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
          mSprite->vy = -VELOCITY;
          break;
        case DIRECTION_DOWN:
          mStep = 1 - mStep;
          mSprite->vy = VELOCITY;
          mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
          break;
        case DIRECTION_LEFT:
          mStep = 1 - mStep;
          mSprite->vx = -VELOCITY;
//          mSprite->mDx = -36;
          mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
          break;
        case DIRECTION_RIGHT:
          mStep = 1 - mStep;
          mSprite->vx = VELOCITY;
          mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
          break;
      }
      break;
    default:
      break;
  }
}

TBool GSpiderProcess::IdleState() {
  return ETrue;
}

TBool GSpiderProcess::WalkState() {
  return ETrue;
}

TBool GSpiderProcess::AttackState() {
  return ETrue;
}

TBool GSpiderProcess::HitState() {
  return ETrue;
}

TBool GSpiderProcess::RunBefore() {
  switch (mState) {
    case IDLE_STATE:
      return IdleState();
    case WALK_STATE:
      return WalkState();
    case ATTACK_STATE:
      return AttackState();
    case HIT_STATE:
      return HitState();
    default:
      return ETrue;
  }
  return ETrue;
}

TBool GSpiderProcess::RunAfter() {
  return ETrue;
}

