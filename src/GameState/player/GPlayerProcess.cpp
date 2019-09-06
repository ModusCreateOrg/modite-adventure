#include "GPlayerProcess.h"
#include "GAnchorSprite.h"
#include "GGamePlayfield.h"
#include "GPlayerAnimations.h"

const TUint16 IDLE_STATE  = 0;
const TUint16 WALK_STATE  = 1;
const TUint16 SWORD_STATE = 2;
const TUint16 FALL_STATE  = 3;

GPlayerProcess::GPlayerProcess(GGameState *aGameState) {
  mGameState = aGameState;
  mPlayfield = ENull;
  mSprite    = new GAnchorSprite(1, PLAYER_SLOT);
  mSprite->type  = STYPE_PLAYER;
  mSprite->cMask = STYPE_ENEMY;
  mSprite->x     = mSprite->y = 32;
  mSprite->w     = 32;
  mSprite->h     = 32;
  mGameState->AddSprite(mSprite);
  mState = IDLE_STATE;
  mStep  = 0;
  mSprite->flags |= SFLAG_ANCHOR | SFLAG_SORTY | SFLAG_CHECK;
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GPlayerProcess::~GPlayerProcess() {
  mSprite->Remove();
  delete mSprite;
}

void GPlayerProcess::StartLevel(
  GGamePlayfield *aPlayfield, TFloat aX, TFloat aY) {
  mPlayfield = aPlayfield;
  mSprite->x = aX;
  mSprite->y = aY;
}

TBool GPlayerProcess::IsWall(TFloat aX, TFloat aY) {
  return mPlayfield->GetAttribute(aX, aY) == ATTR_WALL;
}

TBool GPlayerProcess::IsFloor(TFloat aX, TFloat aY) {
  return mPlayfield->GetAttribute(aX, aY) == ATTR_FLOOR;
}

TBool GPlayerProcess::IsLedge(TFloat aX, TFloat aY) {
  return mPlayfield->GetAttribute(aX, aY) == ATTR_LEDGE;
}

TBool GPlayerProcess::IsLedge(TRect &aRect) {
  return IsLedge(aRect.x1, aRect.y2) || IsLedge(aRect.x2, aRect.y2);
}

TBool GPlayerProcess::IsLedge() {
  return (IsLedge(mSprite->x + mSprite->cx + mSprite->w / 2, mSprite->y + 4));
}

TBool GPlayerProcess::CanWalk(TRect &aRect) {
  if (aRect.x1 < 0 || aRect.y1 < 0) {
    return EFalse;
  }
  if(IsFloor(aRect.x1, aRect.y2) || IsFloor(aRect.x2, aRect.y2)) {
    return ETrue;
  }
  if ( IsLedge(aRect)) {
    const TInt y = mSprite->y;
    mSprite->y = y | 3;
    return ETrue;
  }
}

void GPlayerProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;
  mSprite->mDx        = 0;
  mSprite->mDy        = 0;
  switch (mState) {
    case IDLE_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      switch (mSprite->mDirection) {
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
      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mStep = 1 - mStep;
          mSprite->StartAnimation(mStep ? walkUpAnimation1 : walkUpAnimation2);
          mSprite->vy = -VELOCITY;
          break;
        case DIRECTION_DOWN:
          mStep = 1 - mStep;
          mSprite->vy = VELOCITY;
          mSprite->StartAnimation(
            mStep ? walkDownAnimation1 : walkDownAnimation2);
          break;
        case DIRECTION_LEFT:
          mStep = 1 - mStep;
          mSprite->vx = -VELOCITY;
          //          mSprite->mDx = -36;
          mSprite->StartAnimation(
            mStep ? walkLeftAnimation1 : walkLeftAnimation2);
          break;
        case DIRECTION_RIGHT:
          mStep = 1 - mStep;
          mSprite->vx = VELOCITY;
          mSprite->StartAnimation(
            mStep ? walkRightAnimation1 : walkRightAnimation2);
          break;
      }
      break;
    case SWORD_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mSprite->StartAnimation(swordUpAnimation);
          break;
        case DIRECTION_DOWN:
          mSprite->StartAnimation(swordDownAnimation);
          break;
        case DIRECTION_LEFT:
          mSprite->StartAnimation(swordLeftAnimation);
          break;
        case DIRECTION_RIGHT:
          mSprite->StartAnimation(swordRightAnimation);
          break;
      }
      break;
    case FALL_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      mSprite->StartAnimation(fallAnimation);
      mSprite->mDirection = DIRECTION_DOWN;
      break;
  }
}

/*
  ____ _   _    _    _   _  ____ _____   ____ _____  _  _____ _____
 / ___| | | |  / \  | \ | |/ ___| ____| / ___|_   _|/ \|_   _| ____|
| |   | |_| | / _ \ |  \| | |  _|  _|   \___ \ | | / _ \ | | |  _|
| |___|  _  |/ ___ \| |\  | |_| | |___   ___) || |/ ___ \| | | |___
 \____|_| |_/_/   \_\_| \_|\____|_____| |____/ |_/_/   \_\_| |_____|
 */
TBool GPlayerProcess::MaybeHit() {
  if (mSprite->cType) {
    mSprite->cType = 0;
    return ETrue;
  }
  return EFalse;
}

TBool GPlayerProcess::MaybeSword() {
  if (!gControls.WasPressed(BUTTONA)) {
    return EFalse;
  }
  NewState(SWORD_STATE, mSprite->mDirection);
  return ETrue;
}

TBool GPlayerProcess::MaybeFall() {
  if (IsLedge()) {
    printf("IsLedge");
//  if (IsLedge(mSprite->mRect)) {
    NewState(FALL_STATE, DIRECTION_DOWN);
    return ETrue;
  }
  return EFalse;
}

TBool GPlayerProcess::MaybeWalk() {
  // instead of walking, we might have to attack if A is pressed
  if (MaybeSword()) {
    return ETrue;
  }

  const TFloat x = mSprite->x, y = mSprite->y;
  TRect        r;

  if (gControls.IsPressed(JOYLEFT)) {
    r.Set(x - VELOCITY, y - 18, x + 22 - VELOCITY, y);
    if (!CanWalk(r)) {
      //      mPlayfield->IsWall(mSprite->x + 22 - VELOCITY, mSprite->y) ||
      //          mPlayfield->IsWall(mSprite->x + 22 - VELOCITY, mSprite->y -
      //          18)) {
      return EFalse;
    }

    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_LEFT) {
      NewState(WALK_STATE, DIRECTION_LEFT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYRIGHT)) {
    r.Set(x + 42 + VELOCITY, y, x + 42 + VELOCITY, y - 18);

    if (!CanWalk(r)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_RIGHT) {
      NewState(WALK_STATE, DIRECTION_RIGHT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYUP)) {
    r.Set(x + 22, y - VELOCITY, x + 42, y - 18 - VELOCITY);
    if (!CanWalk(r)) {
      return EFalse;
    }
    //    if (mSprite->y - VELOCITY < 0 || mPlayfield->IsWall(mSprite->x + 22,
    //    mSprite->y - 18 - VELOCITY) ||
    //        mPlayfield->IsWall(mSprite->x + 42, mSprite->y - 18 - VELOCITY)) {
    //      return EFalse;
    //    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_UP) {
      NewState(WALK_STATE, DIRECTION_UP);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYDOWN)) {
    if (MaybeFall()) {
      return EFalse;
    }
    r.Set(x + 22, y + VELOCITY, x + 42, y + VELOCITY);
    if (!CanWalk(r)) {
      return EFalse;
    }
    //    if (mPlayfield->IsWall(mSprite->x + 22, mSprite->y + VELOCITY) ||
    //        mPlayfield->IsWall(mSprite->x + 42, mSprite->y + VELOCITY)) {
    //      return EFalse;
    //    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_DOWN) {
      NewState(WALK_STATE, DIRECTION_DOWN);
    }
    return ETrue;
  }

  return EFalse;
}

/*____ _____  _  _____ _____ ____
/ ___|_   _|/ \|_   _| ____/ ___|
\___ \ | | / _ \ | | |  _| \___ \
 ___) || |/ ___ \| | | |___ ___) |
|____/ |_/_/   \_\_| |_____|____/
*/
TBool GPlayerProcess::IdleState() {
  if (MaybeSword()) {
    return ETrue;
  }
  MaybeWalk();
  return ETrue;
}

TBool GPlayerProcess::WalkState() {
  if (MaybeSword()) {
    return ETrue;
  }

  // maybe change direction!
  if (!MaybeWalk()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  // collision?
  if (MaybeHit()) {
    // bounce player off enemy
    mSprite->x -= mSprite->vx * 2;
    mSprite->y -= mSprite->vy * 2;
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  if (MaybeFall()) {
    NewState(FALL_STATE, DIRECTION_DOWN);
    return ETrue;
  }
  // can player keep walking?
  switch (mSprite->mDirection) {
    case DIRECTION_LEFT:
      //      if (mPlayfield->IsWall(mSprite->x + 22 + mSprite->vx, mSprite->y +
      //      mSprite->vy) ||
      //          mPlayfield->IsWall(mSprite->x + 22 + mSprite->vx, mSprite->y -
      //          18 + mSprite->vy)) {
      if (mPlayfield->IsWall(
        mSprite->x + 26 + mSprite->vx, mSprite->y - 2 + mSprite->vy) ||
          mPlayfield->IsWall(
            mSprite->x + 26 + mSprite->vx, mSprite->y - 8 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      //      if (mPlayfield->IsWall(mSprite->x + 42 + mSprite->vx, mSprite->y +
      //      mSprite->vy) ||
      //          mPlayfield->IsWall(mSprite->x + 42 + mSprite->vx, mSprite->y -
      //          18 + mSprite->vy)) {
      if (mPlayfield->IsWall(
        mSprite->x + 38 + mSprite->vx, mSprite->y - 2 + mSprite->vy) ||
          mPlayfield->IsWall(
            mSprite->x + 38 + mSprite->vx, mSprite->y - 8 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_UP:
      if (mPlayfield->IsWall(mSprite->x + 26, mSprite->y - 8 + mSprite->vy) ||
          mPlayfield->IsWall(mSprite->x + 38, mSprite->y - 8 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_DOWN:
      if (mPlayfield->IsWall(mSprite->x + 26, mSprite->y - 2 + mSprite->vy) ||
          mPlayfield->IsWall(mSprite->x + 38, mSprite->y - 2 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
  }
  if (mSprite->AnimDone()) {
    NewState(WALK_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GPlayerProcess::SwordState() {
  if (mSprite->AnimDone()) {
    NewState(IDLE_STATE, mSprite->mDirection);
  }
  return ETrue;
}

TBool GPlayerProcess::FallState() {
  if (mPlayfield->IsFloor(mSprite->x, mSprite->y)) {
    // land
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }
  mSprite->vy += GRAVITY;
  return ETrue;
}

TBool GPlayerProcess::RunBefore() {
  switch (mState) {
    case IDLE_STATE:
      return IdleState();
    case WALK_STATE:
      return WalkState();
    case SWORD_STATE:
      return SwordState();
    case FALL_STATE:
      return FallState();
    default:
      return ETrue;
  }
}

TBool GPlayerProcess::RunAfter() {
  // position viewport to follow player
  const TFloat xx = mSprite->x - gViewPort->mWorldX,
               yy = mSprite->y - gViewPort->mWorldY,
               dx = gViewPort->mRect.Width(), dy = gViewPort->mRect.Height();

  // Don't go less than 0 or more than map width/height
  if (xx > dx) {
    gViewPort->mWorldX = MIN(mGameState->MapWidth(), gViewPort->mWorldX + dx);
  } else if (xx < 0) {
    gViewPort->mWorldX = MAX(0, gViewPort->mWorldX - dx);
  } else if (yy > dy) {
    gViewPort->mWorldY = MIN(mGameState->MapHeight(), gViewPort->mWorldY + dy);
  } else if (yy < 0) {
    gViewPort->mWorldY = MAX(0, gViewPort->mWorldY - dy);
  }

  return ETrue;
}
