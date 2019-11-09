#include "Game.h"
#include "GAnchorSprite.h"
#include "GGamePlayfield.h"

GAnchorSprite::GAnchorSprite(GGameState *aGameState, TInt aPri, TUint16 aBM, TUint16 aImg, TUint16 aType)
    : BAnimSprite(aPri, aBM, aImg, aType), mName("NO NAME") {

  mGameState = aGameState;
  SetFlags(SFLAG_ANCHOR | SFLAG_SORTPRI);
  if (aType != STYPE_DEFAULT) {
    SetFlags(SFLAG_CHECK);
  }
  mDirection = DIRECTION_DOWN;

  pri = PRIORITY_BELOW;
  mAttributeSave = 0xffff;
  mCollisionDeltaY = DEFAULT_COLLISION_DELTA_Y;
  w = 64;
  h = 64;
  mBaseHitPoints = BASE_HIT_POINTS;
  mBaseStrength = BASE_STRENGTH;
  mBaseExperience = BASE_EXPERIENCE;
  mLevel = 1;
  SetStatMultipliers();
  mLastX = 0;
  mLastY = 0;
  mInvulnerable = EFalse;
  mCollided = ENull;
}

GAnchorSprite::~GAnchorSprite() {
  //
}

void GAnchorSprite::SetAttribute(TUint aAttribute) {
  if (mAttributeSave == 0xffff) {
    mAttributeSave = mGameState->mGamePlayfield->GetAttribute(x, y);
  }
  mGameState->mGamePlayfield->SetAttribute(x, y, aAttribute);
}

// safely position sprite so it's not on top of player
void GAnchorSprite::SafePosition(BSprite *aOther) {
  TRect myRect, hisRect;
  aOther->GetRect(hisRect);
  GetRect(myRect);
  if (!myRect.Overlaps(hisRect)) {
    // already dafe to position here (not on top of other sprite)
    return;
  }
  x = hisRect.x1 - hisRect.Width() - 1;
  if (IsFloor(DIRECTION_LEFT, x, y)) {
    return;
  }
  x = hisRect.x2 + 1;
}

// set or clear wall bits in playfield depending on aState (true = set, false = clear)
void GAnchorSprite::SetWall(TBool aState) {
  TUint16 attribute = mGameState->mGamePlayfield->GetAttribute(x, y);
  if (aState) {
    SetAttribute(ATTR_WALL);
  }
  else {
    SetAttribute(mAttributeSave);
  }
}

TBool GAnchorSprite::IsFloorTile(GAnchorSprite *aSprite, TFloat aX, TFloat aY) {
  TUint16 attr = aSprite->mGameState->mGamePlayfield->GetAttribute(aX, aY);
  return attr == ATTR_FLOOR || (attr == ATTR_LEDGE && aSprite->mDirection != DIRECTION_UP);
}

TBool GAnchorSprite::IsFloor(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
  TRect r;
  GetRect(r);
  r.Offset(aVx, aVy);

  if (r.x1 < 0 || r.y1 < 0) {
    return EFalse;
  }

  switch (aDirection) {
    case DIRECTION_UP:
      if (IsFloorTile(this, r.x1 + FLOOR_ADJUST_LEFT, r.y2 - mCollisionDeltaY) && IsFloorTile(this, r.x2 - FLOOR_ADJUST_RIGHT, r.y2 - mCollisionDeltaY)) {
        return ETrue;
      }
      break;

    case DIRECTION_DOWN:
      if (IsFloorTile(this, r.x1 + FLOOR_ADJUST_LEFT, r.y2) && IsFloorTile(this, r.x2 - FLOOR_ADJUST_RIGHT, r.y2)) {
        return ETrue;
      }
      break;

    case DIRECTION_LEFT:
      if (IsFloorTile(this, r.x1, r.y2 - FLOOR_ADJUST_BOTTOM)) {
        return ETrue;
      }
      break;

    case DIRECTION_RIGHT:
      if (IsFloorTile(this, r.x2, r.y2 - FLOOR_ADJUST_BOTTOM)) {
        return ETrue;
      }
      break;
  }

  return EFalse;
}

void GAnchorSprite::Move() {
  mLastX = x;
  mLastY = y;
  BAnimSprite::Move();
  if (!TestFlags(SFLAG_BELOW)) {
    pri = y + 1000;
  }
}

void GAnchorSprite::Collide(BSprite *aOther) {
  auto *s = (GAnchorSprite *)aOther;
  TBool collided = EFalse;
  if (type == STYPE_PBULLET || type == STYPE_EBULLET || s->type == STYPE_PBULLET || s->type == STYPE_EBULLET) {
    collided = ETrue;
  }

  if (s->TestFlags(SFLAG_COLLIDE2D)) {
    collided = ETrue;
  }
  else if (ABS(s->y - y) < mCollisionDeltaY) {
    collided = ETrue;
  }

  if (collided) {
    mCollided = s;
    s->mCollided = this;
    cType |= s->type;
    s->cType |= type;
  }
}

void GAnchorSprite::Nudge() {
  vx = vy = 0;
  x = mLastX;
  y = mLastY;
}

TBool GAnchorSprite::Render(BViewPort *aViewPort) {
  TBool ret = BAnimSprite::Render(aViewPort);

#ifdef DEBUG_MODE
  if (GGame::mDebug && !Clipped()) {
    gDisplay.renderBitmap->SetColor(COLOR_TEXT, 255, 0, 255);
    gDisplay.renderBitmap->DrawRect(aViewPort, mRect, COLOR_TEXT);
    // render collision rect
    TRect r;
    GetRect(r);
    r.x1 -= aViewPort->mWorldX;
    r.x2 -= aViewPort->mWorldX;
    r.y1 -= aViewPort->mWorldY;
    r.y2 -= aViewPort->mWorldY;
    gDisplay.SetColor(COLOR_TEXT_SHADOW, 255, 0, 0);
    gDisplay.renderBitmap->DrawRect(aViewPort, r, COLOR_TEXT_SHADOW);

    gDisplay.renderBitmap->DrawFastHLine(aViewPort, mRect.x1 - 5, mRect.y2, 10, COLOR_TEXT_SHADOW);
    gDisplay.renderBitmap->DrawFastVLine(aViewPort, mRect.x1, mRect.y2 - 5, 10, COLOR_TEXT_SHADOW);
  }
#endif

  return ret;
}

DIRECTION GAnchorSprite::RandomDirection() {
  TInt dir = Random() & TUint8(3);
  switch (dir) {
    case 0:
      return DIRECTION_UP;
    case 1:
      return DIRECTION_DOWN;
    case 2:
      return DIRECTION_LEFT;
    case 3:
    default:
      return DIRECTION_RIGHT;
  }
}

TBool GAnchorSprite::CanWalk(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
  return IsFloor(aDirection, aVx, aVy);
}

void GAnchorSprite::SetStatMultipliers(TFloat aModHitPoints, TFloat aModStrength, TFloat aModExperience) {
  mBaseHitPoints = round(aModHitPoints * BASE_HIT_POINTS);
  mBaseStrength = round(aModStrength * BASE_STRENGTH);
  mBaseExperience = round(aModExperience * BASE_EXPERIENCE);
  SetLevel(mLevel);
}

void GAnchorSprite::SetLevel(TInt aLevel) {
  mLevel = aLevel;
  mMaxHitPoints = mBaseHitPoints + mLevel * (mBaseHitPoints / 5);
  mHitPoints = mMaxHitPoints;
  mHitStrength = mBaseStrength + mLevel * (mBaseStrength / 5);
  mExperience = mBaseExperience + mLevel * (mBaseExperience / 5);
}
