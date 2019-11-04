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
  mHitStrength = HIT_LIGHT;

  w = 64;
  h = 64;
  mLevel = 1;
  mNextLevel = 0;
  mExperience = 0;
  mHitPoints = 5;
  mStrength = 10;
  mDexterity = 10;
  mGold = 0;
  mLastX = 0;
  mLastY = 0;
  mInvulnerable = EFalse;
  mCollided = ENull;
  mShadow = TRect();
}

GAnchorSprite::~GAnchorSprite() {
  //
}

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
      if (IsFloorTile(this, r.x1 + FLOOR_ADJUST_LEFT, r.y1) && IsFloorTile(this, r.x2 - FLOOR_ADJUST_RIGHT, r.y1)) {
        return ETrue;
      }
      break;

    case DIRECTION_DOWN:
      if (IsFloorTile(this, r.x1 + FLOOR_ADJUST_LEFT, r.y2) && IsFloorTile(this, r.x2 - FLOOR_ADJUST_RIGHT, r.y2)) {
        return ETrue;
      }
      break;

    case DIRECTION_LEFT:
      if (IsFloorTile(this, r.x1, r.y1 + FLOOR_ADJUST_TOP) && IsFloorTile(this, r.x1, r.y2 - FLOOR_ADJUST_BOTTOM)) {
        return ETrue;
      }
      break;

    case DIRECTION_RIGHT:
      if (IsFloorTile(this, r.x2, r.y1 + FLOOR_ADJUST_TOP) && IsFloorTile(this, r.x2, r.y2 - FLOOR_ADJUST_BOTTOM)) {
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
  if (TestFlags(SFLAG_BELOW)) {
    pri = 999;
  }
  else {
    pri = y + 1000;
  }
}

void GAnchorSprite::Collide(BSprite *aOther) {
  auto *s = (GAnchorSprite *)aOther;
  mCollided = s;
  s->mCollided = this;
  cType |= s->type;
  s->cType |= type;
}

void GAnchorSprite::Nudge() {
  vx = vy = 0;
  x = mLastX;
  y = mLastY;
}

void GAnchorSprite::ResetShadow() {
  mShadow.Set(cx, cy - h, cx + w, cy);
}

TBool GAnchorSprite::Render(BViewPort *aViewPort) {
  if ((flags & SFLAG_RENDER_SHADOW) && !Clipped()) {
    if (mShadow.x1 == 0 && mShadow.x2 == 0 && mShadow.y1 == 0 && mShadow.y2 == 0) {
      ResetShadow();
    }
    gDisplay.renderBitmap->SetColor(COLOR_SHADOW, 40, 40, 60);

    for (TInt i = mShadow.y1; i < mShadow.y2; i++) {
      TFloat chord = sqrt(pow(mShadow.Height() / 2, 2) - pow(i - ((mShadow.y1 + mShadow.y2) / 2), 2)) * 2 * mShadow.Width() / mShadow.Height();
      gDisplay.renderBitmap->DrawFastHLine(aViewPort, mRect.x1 + mShadow.x1 + mShadow.Width() - chord/2, mRect.y2 + i, chord, COLOR_SHADOW);
    }
  }
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
