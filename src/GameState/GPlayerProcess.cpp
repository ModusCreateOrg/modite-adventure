#include "GPlayerProcess.h"
#include "GGamePlayfield.h"

const TUint16 IDLE_STATE = 0;
const TUint16 WALK_STATE = 1;

enum {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
};

static ANIMSCRIPT idleAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ALABEL,
  ASTEP(40, IMG_IDLE),
  ASTEP(4, IMG_IDLE + 1),
  ASTEP(40, IMG_IDLE + 2),
  ASTEP(4, IMG_IDLE + 1),
  ALOOP
};

static ANIMSCRIPT selectedAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ALABEL,
  ASTEP(40, IMG_SELECTED),
  ASTEP(4, IMG_SELECTED + 1),
  ASTEP(40, IMG_SELECTED + 2),
  ASTEP(4, IMG_SELECTED + 1),
  ALOOP
};

#define WALKSPEED 5
static ANIMSCRIPT walkDownAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(WALKSPEED, IMG_WALK_DOWN),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 1),
  AEND
};

static ANIMSCRIPT walkDownAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 2),
  ASTEP(WALKSPEED, IMG_WALK_DOWN + 3),
  AEND
};

static ANIMSCRIPT walkLeftAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  AFLIP(WALKSPEED, IMG_WALK_LEFT_RIGHT),
  AFLIP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkLeftAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  AFLIP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 2),
  AFLIP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 3),
  AEND
};

static ANIMSCRIPT walkRightAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(WALKSPEED, IMG_WALK_LEFT_RIGHT),
  ASTEP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 1),
  AEND
};

static ANIMSCRIPT walkRightAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 2),
  ASTEP(WALKSPEED, IMG_WALK_LEFT_RIGHT + 3),
  AEND
};

static ANIMSCRIPT walkUpAnimation1[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(WALKSPEED, IMG_WALK_UP),
  ASTEP(WALKSPEED, IMG_WALK_UP + 1),
  AEND
};

static ANIMSCRIPT walkUpAnimation2[] = {
  ABITMAP(PLAYER_SLOT),
  ASTEP(WALKSPEED, IMG_WALK_UP + 2),
  ASTEP(WALKSPEED, IMG_WALK_UP + 3),
  AEND
};

GPlayerProcess::GPlayerProcess(GGameState *aGameState, GGamePlayfield *aPlayfield) {
  mGameState = aGameState;
  mPlayfield = aPlayfield;
  BBitmap *bm = gResourceManager.GetBitmap(PLAYER_SLOT);
  TRGB *pal = bm->GetPalette();
  for (TInt i = 128; i < 192; i++) {
    gDisplay.SetColor(i, pal[i]);
  }
  mSprite = new BAnimSprite(1, PLAYER_SLOT);
  mSprite->x = mSprite->y = 32;
  mGameState->AddSprite(mSprite);
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GPlayerProcess::~GPlayerProcess() {
  //
}

#define VELOCITY (4)

void GPlayerProcess::NewState(TUint16 aState, TUint16 aDirection) {
  mState = aState;
  mDirection = aDirection;
  mSprite->mDx = 0;
  mSprite->mDy = 0;
  switch (mState) {
    case IDLE_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mSprite->StartAnimation(idleAnimation);
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
          mSprite->mDx = -36;
          mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
          break;
        case DIRECTION_RIGHT:
          mStep = 1 - mStep;
          mSprite->vx = VELOCITY;
          mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
          break;
      }
  }
}

TBool GPlayerProcess::MaybeWalk() {
  if (gControls.IsPressed(JOYLEFT)) {
    if (mPlayfield->IsWall(mSprite->x - VELOCITY, mSprite->y)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mDirection != DIRECTION_LEFT) {
      NewState(WALK_STATE, DIRECTION_LEFT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYRIGHT)) {
    if (mPlayfield->IsWall(mSprite->x + 28 + VELOCITY, mSprite->y)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mDirection != DIRECTION_RIGHT) {
      NewState(WALK_STATE, DIRECTION_RIGHT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYUP)) {
    if (mPlayfield->IsWall(mSprite->x + 2, mSprite->y - VELOCITY)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mDirection != DIRECTION_UP) {
      NewState(WALK_STATE, DIRECTION_UP);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYDOWN)) {
    if (mPlayfield->IsWall(mSprite->x + 2, mSprite->y + 28 + VELOCITY)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mDirection != DIRECTION_DOWN) {
      NewState(WALK_STATE, DIRECTION_DOWN);
    }
    return ETrue;
  }

  return EFalse;
}

TBool GPlayerProcess::IdleState() {
  MaybeWalk();
  return ETrue;
}

TBool GPlayerProcess::WalkState() {
  if (!MaybeWalk()) {
    NewState(IDLE_STATE, DIRECTION_DOWN);
    return ETrue;
  }

  // can player keep walking?
  switch (mDirection) {
    case DIRECTION_LEFT:
      if (mPlayfield->IsWall(mSprite->x + mSprite->vx, mSprite->y + mSprite->vy)) {
        NewState(IDLE_STATE, DIRECTION_DOWN);
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      if (mPlayfield->IsWall(mSprite->x + mSprite->vx + 30, mSprite->y + mSprite->vy)) {
        NewState(IDLE_STATE, DIRECTION_DOWN);
        return ETrue;
      }
      break;
    case DIRECTION_UP:
      if (mPlayfield->IsWall(mSprite->x + 2 + mSprite->vx, mSprite->y + mSprite->vy)) {
        NewState(IDLE_STATE, DIRECTION_DOWN);
        return ETrue;
      }
      break;
    case DIRECTION_DOWN:
      if (mPlayfield->IsWall(mSprite->x + 2 + mSprite->vx, mSprite->y + mSprite->vy + 16)) {
        printf("IDLE\n");
        NewState(IDLE_STATE, DIRECTION_DOWN);
        return ETrue;
      }
      break;
  }

  // if animation is done, start the other foot animation
  if (mSprite->AnimDone()) {
    NewState(WALK_STATE, mDirection);
  }
  return ETrue;
}

TBool GPlayerProcess::RunBefore() {
  switch (mState) {
    case IDLE_STATE:
      return IdleState();
    case WALK_STATE:
      return WalkState();
    default:
      return ETrue;
  }
}

TBool GPlayerProcess::RunAfter() {
  // position viewport to follow player
  const TFloat xx = mSprite->x - gViewPort->mWorldX,
    yy = mSprite->y - gViewPort->mWorldY,
    dx = gViewPort->mRect.Width(),
    dy = gViewPort->mRect.Height();

  if (xx > dx) {
    gViewPort->mWorldX += dx;
  } else if (xx < 0) {
    gViewPort->mWorldX -= dx;
  } else if (yy > dy) {
    gViewPort->mWorldY += dy;
  } else if (yy < 0) {
    gViewPort->mWorldY -= dy;
  }
  return ETrue;
}
