#include "GPlayerProcess.h"
#include "GAnchorSprite.h"
#include "GGamePlayfield.h"
#include "GPlayerAnimations.h"
#include "GStatProcess.h"

const TInt PLAYER_HITPOINTS = 10;

const TUint16 IDLE_STATE = 0;
const TUint16 WALK_STATE = 1;
const TUint16 SWORD_STATE = 2;
const TUint16 SWORD_NO_BULLET_STATE = 3;
const TUint16 FALL_STATE = 4;
const TUint16 HIT_LIGHT_STATE = 5;
const TUint16 HIT_MEDIUM_STATE = 6;
const TUint16 HIT_HARD_STATE = 7;

GPlayerProcess::GPlayerProcess(GGameState *aGameState) {
  mGameState = aGameState;
  mPlayfield = ENull;
  GPlayer::mSprite = mSprite = ENull;
  GPlayer::mSprite = mSprite = new GAnchorSprite(mGameState, -100, PLAYER_SLOT);
  mSprite->Name("PLAYER SPRITE");
  GPlayer::mHitPoints = PLAYER_HITPOINTS;
  mGameState->AddSprite(mSprite);
  mSprite->type = STYPE_PLAYER;
  mSprite->cMask = STYPE_ENEMY | STYPE_EBULLET | STYPE_OBJECT; // collide with enemy, enemy attacks, and environment
  mSprite->w = 32;
  mSprite->h = 32;
  mSprite->SetFlags(SFLAG_ANCHOR | SFLAG_CHECK); // SFLAG_SORTY
  NewState(IDLE_STATE, DIRECTION_DOWN);
}

GPlayerProcess::~GPlayerProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    GPlayer::mSprite = mSprite = ENull;
  }
}

void GPlayerProcess::StartLevel(GGamePlayfield *aPlayfield, TFloat aX, TFloat aY) {
  mPlayfield = aPlayfield;
  mSprite->x = aX;
  mSprite->y = aY;
}

TBool GPlayerProcess::IsWall(TFloat aX, TFloat aY) {
  TUint16 attr = mPlayfield->GetAttribute(aX, aY);
  return attr == ATTR_WALL; // || (attr == ATTR_LEDGE && mSprite->mDirection==
  // DIRECTION_UP);
}

TBool GPlayerProcess::IsFloor(TFloat aX, TFloat aY) {
  TUint16 attr = mPlayfield->GetAttribute(aX, aY);
  return attr == ATTR_FLOOR ||
         (attr == ATTR_LEDGE && mSprite->mDirection != DIRECTION_UP);
}

TBool GPlayerProcess::IsLedge(TFloat aX, TFloat aY) {
  return mPlayfield->GetAttribute(aX, aY) == ATTR_LEDGE && (TInt(aY) % 32 > 12);
}

TBool GPlayerProcess::IsLedge(TRect &aRect) {
  return IsLedge(aRect.x1, aRect.y2) || IsLedge(aRect.x2, aRect.y2);
}

TBool GPlayerProcess::IsLedge() {
  return (IsLedge(mSprite->x + mSprite->cx + mSprite->w / 2, mSprite->y + 4));
}

TBool GPlayerProcess::CanWalk(DIRECTION aDirection) {
  TRect r;
  mSprite->GetRect(r);

  if (r.x1 < 0 || r.y1 < 0) {
    return EFalse;
  }

  switch (aDirection) {
    case DIRECTION_UP:
      r.Offset(0, -PLAYER_VELOCITY);
      if (IsFloor(r.x1 + 2, r.y1) && IsFloor(r.x2 - 2, r.y1)) {
        return ETrue;
      }
      break;
    case DIRECTION_DOWN:
      r.Offset(0, PLAYER_VELOCITY);
      if (IsFloor(r.x1 + 2, r.y2) && IsFloor(r.x2 - 2, r.y2)) {
        return ETrue;
      }
      break;
    case DIRECTION_LEFT:
      r.Offset(-PLAYER_VELOCITY, 0);
      if (IsFloor(r.x1, r.y1 + 2) && IsFloor(r.x1, r.y2 - 2)) {
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      r.Offset(PLAYER_VELOCITY, 0);
      if (IsFloor(r.x2, r.y1 + 2) && IsFloor(r.x2, r.y2 - 2)) {
        return ETrue;
      }
      break;
  }

  if (IsLedge(r)) {
    const TInt y = mSprite->y;
    mSprite->y = y | 3;
    return ETrue;
  }

  return EFalse;
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
          mSprite->vy = -PLAYER_VELOCITY;
          break;
        case DIRECTION_DOWN:
          mStep = 1 - mStep;
          mSprite->vy = PLAYER_VELOCITY;
          mSprite->StartAnimation(mStep ? walkDownAnimation1 : walkDownAnimation2);
          break;
        case DIRECTION_LEFT:
          mStep = 1 - mStep;
          mSprite->vx = -PLAYER_VELOCITY;
          //          mSprite->mDx = -36;
          mSprite->StartAnimation(mStep ? walkLeftAnimation1 : walkLeftAnimation2);
          break;
        case DIRECTION_RIGHT:
          mStep = 1 - mStep;
          mSprite->vx = PLAYER_VELOCITY;
          mSprite->StartAnimation(mStep ? walkRightAnimation1 : walkRightAnimation2);
          break;
      }
      break;
    case SWORD_STATE:
    case SWORD_NO_BULLET_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = 0;
      // TODO: calculate hit strengh based upon leven and strength
      mSprite->mHitStrength = 1;
      gSoundPlayer.SfxBadDrop();
      switch (mSprite->mDirection) {
        case DIRECTION_UP:
          mSprite->StartAnimation(mState == SWORD_NO_BULLET_STATE ? swordUpNoBulletAnimation : swordUpAnimation);
          break;
        case DIRECTION_DOWN:
          mSprite->StartAnimation(mState == SWORD_NO_BULLET_STATE ? swordDownNoBulletAnimation : swordDownAnimation);
          break;
        case DIRECTION_LEFT:
          mSprite->StartAnimation(mState == SWORD_NO_BULLET_STATE ? swordLeftNoBulletAnimation : swordLeftAnimation);
          break;
        case DIRECTION_RIGHT:
          mSprite->StartAnimation(mState == SWORD_NO_BULLET_STATE ? swordRightNoBulletAnimation : swordRightAnimation);
          break;
      }
      mState = SWORD_STATE;
      break;
    case FALL_STATE:
      mStep = 0;
      mSprite->vx = 0;
      mSprite->vy = GRAVITY;
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
#ifdef DEBUGME
  printf("Player collide cType: %x STYPE_ENEMY: %x STYPE_EBULLET: %x\n",
      mSprite->cType, STYPE_ENEMY, STYPE_EBULLET);
#endif

  if (mSprite->cType & STYPE_OBJECT) {
    mSprite->cType &= ~STYPE_OBJECT;
  }
  if (mSprite->mInvulnerable) {
    mSprite->cType &= STYPE_EBULLET;
  }

  if (mSprite->cType & STYPE_EBULLET) {
    mSprite->cType &= STYPE_EBULLET;
    mSprite->Nudge();
    TInt state = HIT_LIGHT_STATE;
#ifdef DEBUGME
    printf("Player attacked\n");
#endif
    const GAnchorSprite *other = mSprite->mCollided;
    switch (other->mHitStrength) {

      case HIT_LIGHT:
        GPlayer::mHitPoints -= 1;
        mSprite->mInvulnerable = ETrue;
        mGameState->AddProcess(new GStatProcess(mSprite->x - 32, mSprite->y - 63, "HIT +1"));
        switch (other->mDirection) {
          case DIRECTION_UP:
            mSprite->StartAnimation(hitLightUpAnimation);
            break;
          case DIRECTION_DOWN:
            mSprite->StartAnimation(hitLightDownAnimation);
            break;
          case DIRECTION_LEFT:
            mSprite->StartAnimation(hitLightLeftAnimation);
            break;
          case DIRECTION_RIGHT:
            mSprite->StartAnimation(hitLightRightAnimation);
            break;
        }
        break;

      case HIT_MEDIUM:
        GPlayer::mHitPoints -= 2;
        mSprite->mInvulnerable = ETrue;
        state = HIT_MEDIUM_STATE;
        mGameState->AddProcess(new GStatProcess(mSprite->x - 32, mSprite->y - 63, "HIT +2"));
        switch (other->mDirection) {
          case DIRECTION_UP:
            mSprite->StartAnimation(hitMediumUpAnimation);
            break;
          case DIRECTION_DOWN:
            mSprite->StartAnimation(hitMediumDownAnimation);
            break;
          case DIRECTION_LEFT:
            mSprite->StartAnimation(hitMediumLeftAnimation);
            break;
          case DIRECTION_RIGHT:
            mSprite->StartAnimation(hitMediumRightAnimation);
            break;
        }
        break;

      case HIT_HARD:
        GPlayer::mHitPoints -= 3;
        mSprite->mInvulnerable = ETrue;
        state = HIT_HARD_STATE;
        mGameState->AddProcess(new GStatProcess(mSprite->x+64, mSprite->y, "HIT +3"));
        switch (other->mDirection) {
          case DIRECTION_UP:
            mSprite->StartAnimation(hitHardUpAnimation);
            break;
          case DIRECTION_DOWN:
            mSprite->StartAnimation(hitHardDownAnimation);
            break;
          case DIRECTION_LEFT:
            mSprite->StartAnimation(hitHardLeftAnimation);
            break;
          case DIRECTION_RIGHT:
            mSprite->StartAnimation(hitHardRightAnimation);
            break;
        }
        break;
    }
    mState = state;

    if (GPlayer::mHitPoints <= 0) {
      // GAME OVER!
#ifdef DEBUGME
      printf("Player dead\n");
#endif
      GPlayer::mHitPoints = PLAYER_HITPOINTS;
    }
    mSprite->cType = 0;
    return ETrue;
  }

  if (mSprite->cType & STYPE_ENEMY) {
    mSprite->cType &= ~STYPE_ENEMY;
    mSprite->Nudge();
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  return EFalse;
}

TBool GPlayerProcess::MaybeSword() {
  if (!gControls.WasPressed(BUTTONA)) {
    return EFalse;
  }
  TFloat x = mSprite->x, y = mSprite->y;
  TRect r;
  mSprite->GetRect(r);

  TBool is_wall = EFalse;
  switch (mSprite->mDirection) {
    case DIRECTION_UP:
      r.Offset(0, -18);
      if (!IsFloor(r.x1 + r.Width() / 2, r.y1 - 8)) {
        is_wall = ETrue;
      }
      break;
    case DIRECTION_DOWN:
      r.Offset(0, 24);
      if (!IsFloor(x + 32, r.y2)) {
        is_wall = ETrue;
      }
      break;
    case DIRECTION_LEFT:
      r.Offset(-1, 0);
      if (!IsFloor(r.x1, r.y2)) {
        is_wall = ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      r.Offset(24, 0);
      if (!IsFloor(r.x2, r.y2)) {
        is_wall = ETrue;
      }
      break;
  }
  NewState(is_wall ? SWORD_NO_BULLET_STATE : SWORD_STATE, mSprite->mDirection);
  return ETrue;
}

TBool GPlayerProcess::MaybeFall() {
  if (IsLedge()) {
    printf("IsLedge\n");
    //  if (IsLedge(mSprite->mRect)) {
    NewState(FALL_STATE, DIRECTION_DOWN);
    return ETrue;
  }
  return EFalse;
}

TBool GPlayerProcess::MaybeWalk() {
  if (gControls.IsPressed(JOYLEFT)) {
    if (!CanWalk(DIRECTION_LEFT)) {
//      mSprite->vx = mSprite->vy = 0;
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_LEFT) {
      NewState(WALK_STATE, DIRECTION_LEFT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYRIGHT)) {
    if (!CanWalk(DIRECTION_RIGHT)) {
//      mSprite->vx = mSprite->vy = 0;
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_RIGHT) {
      NewState(WALK_STATE, DIRECTION_RIGHT);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYUP)) {
    if (!CanWalk(DIRECTION_UP)) {
//      mSprite->vx = mSprite->vy = 0;
      return EFalse;
    }
    if (mState != WALK_STATE || mSprite->mDirection != DIRECTION_UP) {
      NewState(WALK_STATE, DIRECTION_UP);
    }
    return ETrue;
  }

  if (gControls.IsPressed(JOYDOWN)) {
    if (MaybeFall()) {
      return EFalse;
    }
    if (!CanWalk(DIRECTION_DOWN)) {
//      mSprite->vx = mSprite->vy = 0;
      return EFalse;
    }
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
  // collision?
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeSword()) {
    return ETrue;
  }

  MaybeWalk();
  return ETrue;
}

TBool GPlayerProcess::WalkState() {
  // collision?
  if (MaybeHit()) {
    return ETrue;
  }

  if (MaybeSword()) {
    return ETrue;
  }

  // maybe change direction!
  if (!MaybeWalk()) {
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }

  //  if (MaybeFall()) {
  //    NewState(FALL_STATE, DIRECTION_DOWN);
  //    return ETrue;
  //  }
  // can player keep walking?
  switch (mSprite->mDirection) {
    case DIRECTION_LEFT:
      if (mPlayfield->IsWall(mSprite->x + 26 + mSprite->vx, mSprite->y - 2 + mSprite->vy) ||
          mPlayfield->IsWall(mSprite->x + 26 + mSprite->vx, mSprite->y - 8 + mSprite->vy)) {
        NewState(IDLE_STATE, mSprite->mDirection);
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      if (mPlayfield->IsWall(mSprite->x + 38 + mSprite->vx, mSprite->y - 2 + mSprite->vy) ||
          mPlayfield->IsWall(mSprite->x + 38 + mSprite->vx, mSprite->y - 8 + mSprite->vy)) {
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
  if (mPlayfield->IsFloor(mSprite->x + 32, mSprite->y + mSprite->vy)) {
    // land
    NewState(IDLE_STATE, mSprite->mDirection);
    return ETrue;
  }
  mSprite->vy += GRAVITY;
  return ETrue;
}

TBool GPlayerProcess::HitState() {
  if (mSprite->AnimDone()) {
    mSprite->cType &= ~STYPE_EBULLET;
    mSprite->mInvulnerable = EFalse;
    if (!MaybeWalk()) {
      NewState(IDLE_STATE, mSprite->mDirection);
    }
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
    case FALL_STATE:
      return FallState();
    case HIT_HARD_STATE:
    case HIT_MEDIUM_STATE:
    case HIT_LIGHT_STATE:
      return HitState();
    default:
      return ETrue;
  }
}

#define SMOOTH_SCROLLING

TBool GPlayerProcess::RunAfter() {
  // position viewport to follow player
#ifdef SMOOTH_SCROLLING
  TFloat maxx = mGameState->MapWidth(),
    maxy = mGameState->MapHeight();

  // half viewport size
  const TFloat ww = gViewPort->mRect.Width() / 2.0,
    hh = gViewPort->mRect.Height() / 2.0;

  // upper left corner of desired viewport position
  TFloat xx = gViewPort->mWorldX = mSprite->x - ww,
    yy = gViewPort->mWorldY = mSprite->y - hh;

  if (xx < 0) {
    gViewPort->mWorldX = 0;
  } else if (xx > maxx) {
    gViewPort->mWorldX = maxx;
  }
  if (yy < 0) {
    gViewPort->mWorldY = 0;
  } else if (yy > maxy) {
    gViewPort->mWorldY = maxy;
  }
#else
  const TFloat xx = mSprite->x - gViewPort->mWorldX,
               yy = mSprite->y - gViewPort->mWorldY,
               dx = gViewPort->mRect.Width(), dy = gViewPort->mRect.Height();

  // Don't go less than 0 or more than map width/height
  if (xx > dx) {
    gViewPort->mWorldX = MIN(mGameState->MapWidth(), gViewPort->mWorldX + dx);
  }
  else if (xx < 0) {
    gViewPort->mWorldX = MAX(0, gViewPort->mWorldX - dx);
  }
  else if (yy > dy) {
    gViewPort->mWorldY = MIN(mGameState->MapHeight(), gViewPort->mWorldY + dy);
  }
  else if (yy < 0) {
    gViewPort->mWorldY = MAX(0, gViewPort->mWorldY - dy);
  }
#endif

  return ETrue;
}
