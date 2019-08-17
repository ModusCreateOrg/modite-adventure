#include "GPlayerProcess.h"
#include "GAnchorSprite.h"
#include "GGamePlayfield.h"
#include "GPlayerAnimations.h"

const TUint16 IDLE_STATE = 0;
const TUint16 WALK_STATE = 1;
const TUint16 SWORD_STATE = 2;

GPlayerProcess::GPlayerProcess(GGameState *aGameState) {
  mGameState = aGameState;
  mPlayfield = ENull;
  mSprite = new GAnchorSprite(1, PLAYER_SLOT);
  mSprite->type = STYPE_PLAYER;
  mSprite->cMask = STYPE_ENEMY;
  mSprite->x = mSprite->y = 32;
  mSprite->w = 32;
  mSprite->h = 32;
  mGameState->AddSprite(mSprite);
  mSprite->flags |= SFLAG_ANCHOR | SFLAG_SORTY | SFLAG_CHECK;
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GPlayerProcess::~GPlayerProcess() {
  //
}

void GPlayerProcess::StartLevel(GGamePlayfield *aPlayfield, TFloat aX, TFloat aY) {
  mPlayfield = aPlayfield;
  mSprite->x = aX;
  mSprite->y = aY;
//  BBitmap *bm = gResourceManager.GetBitmap(PLAYER_SLOT);
////  bm->Remap(PLAYER_PALETTE, PLAYER_COLORS);
//  TRGB *pal = bm->GetPalette();
//  for (TInt i = PLAYER_PALETTE; i < PLAYER_PALETTE+PLAYER_COLORS-1; i++) {
//    gDisplay.SetColor(i, pal[i]);
//  }
}

void GPlayerProcess::NewState(TUint16 aState, DIRECTION aDirection) {
  mState = aState;
  mSprite->mDirection = aDirection;
  mSprite->mDx = 0;
  mSprite->mDy = 0;
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
  }
}

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

TBool GPlayerProcess::MaybeWalk() {
  // instead of walking, we might have to attack if A is pressed
  if (MaybeSword()) {
    return ETrue;
  }
  if (gControls.IsPressed(JOYLEFT)) {
    if (mSprite->x - VELOCITY < 0 || mPlayfield->IsWall(mSprite->x + 22 - VELOCITY, mSprite->y) || mPlayfield->IsWall(mSprite->x + 22 - VELOCITY, mSprite->y - 18)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_LEFT) {
      NewState(WALK_STATE, DIRECTION_LEFT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYRIGHT)) {
    if (mPlayfield->IsWall(mSprite->x + 42 + VELOCITY, mSprite->y) || mPlayfield->IsWall(mSprite->x + 42 + VELOCITY, mSprite->y - 18)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_RIGHT) {
      NewState(WALK_STATE, DIRECTION_RIGHT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYUP)) {
    if (mSprite->y - VELOCITY < 0 || mPlayfield->IsWall(mSprite->x + 22, mSprite->y - 18 - VELOCITY) || mPlayfield->IsWall(mSprite->x + 42, mSprite->y - 18 - VELOCITY)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_UP) {
      NewState(WALK_STATE, DIRECTION_UP);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYDOWN)) {
    if (mPlayfield->IsWall(mSprite->x + 22 , mSprite->y + VELOCITY) || mPlayfield->IsWall(mSprite->x + 42 , mSprite->y + VELOCITY)) {
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_DOWN) {
      NewState(WALK_STATE, DIRECTION_DOWN);
    }
    return ETrue;
  }

  return EFalse;
}

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
    mSprite->x -= mSprite->vx*2;
    mSprite->y -= mSprite->vy*2;
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  // can player keep walking?
  switch (mSprite->mDirection) {
    case DIRECTION_LEFT:
      if (mPlayfield->IsWall(mSprite->x + 22 + mSprite->vx, mSprite->y + mSprite->vy) || mPlayfield->IsWall(mSprite->x + 22 + mSprite->vx, mSprite->y - 18 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      if (mPlayfield->IsWall(mSprite->x + 42 + mSprite->vx , mSprite->y + mSprite->vy) || mPlayfield->IsWall(mSprite->x + 42 + mSprite->vx , mSprite->y - 18 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_UP:
      if (mPlayfield->IsWall(mSprite->x + 22, mSprite->y - 18 + mSprite->vy) || mPlayfield->IsWall(mSprite->x + 42 , mSprite->y - 18 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_DOWN:
      if (mPlayfield->IsWall(mSprite->x + 22, mSprite->y + mSprite->vy) || mPlayfield->IsWall(mSprite->x + 42, mSprite->y + mSprite->vy)) {
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

TBool GPlayerProcess::RunBefore() {
  switch (mState) {
    case IDLE_STATE:
      return IdleState();
    case WALK_STATE:
      return WalkState();
    case SWORD_STATE:
      return SwordState();
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
