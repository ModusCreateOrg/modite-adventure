#include <GameState/player/GPlayer.h>
#include <GameState/inventory/Items.h>
#include <GameState/status/GStatProcess.h>
#include "Game.h"
#include "GAnchorSprite.h"
#include "GGamePlayfield.h"

// constructor
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
  mShadow = TRect();
  mElement = ELEMENT_NONE;
  gDisplay.renderBitmap->SetColor(COLOR_SHADOW, 40, 40, 60);

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
  if (CanWalk(0, 0, ETrue)) {
    return;
  }
  x = hisRect.x2 + 1;
}

// set or clear wall bits in playfield depending on aState (true = set, false = clear)
void GAnchorSprite::SetWall(TBool aState) {
  TUint16 attribute = mGameState->mGamePlayfield->GetAttribute(x, y);
  if (aState) {
    SetAttribute(0);
  }
  else {
    SetAttribute(mAttributeSave);
  }
}

TBool GAnchorSprite::IsFloorTile(TFloat aX, TFloat aY) {
  return mGameState->mGamePlayfield->IsFloor(aX, aY);
}

TBool GAnchorSprite::CanWalkInDirection(DIRECTION aDirection, TFloat aDx, TFloat aDy) {
  GFloatRect r;
  GetFloatRect(r);
  r.Offset(aDx, aDy);

  switch (aDirection) {
    case DIRECTION_UP:
      for (TInt i = r.x1; i < r.x2; i += WALL_THICKNESS) {
        if (!IsFloorTile(i, r.y1)) {
          return EFalse;
        }
      }
      return IsFloorTile(r.x2, r.y1);
    case DIRECTION_DOWN:
      for (TInt i = r.x1; i < r.x2; i += WALL_THICKNESS) {
        if (!IsFloorTile(i, r.y2)) {
          return EFalse;
        }
      }
      return IsFloorTile(r.x2, r.y2);
    case DIRECTION_LEFT:
      for (TInt i = r.y1; i < r.y2; i += WALL_THICKNESS) {
        if (!IsFloorTile(r.x1, i)) {
          return EFalse;
        }
      }
      return IsFloorTile(r.x1, r.y2);
    case DIRECTION_RIGHT:
      for (TInt i = r.y1; i < r.y2; i += WALL_THICKNESS) {
        if (!IsFloorTile(r.x2, i)) {
          return EFalse;
        }
      }
      return IsFloorTile(r.x2, r.y2);
    default:
      return EFalse;
  }
}

TBool GAnchorSprite::CanWalk(TFloat aDx, TFloat aDy, TBool aCheckAllSides) {
  if ((aDy < 0 || aCheckAllSides) && !CanWalkInDirection(DIRECTION_UP, aDx, aDy)) {
    return EFalse;
  }
  if ((aDy > 0 || aCheckAllSides) && !CanWalkInDirection(DIRECTION_DOWN, aDx, aDy)) {
    return EFalse;
  }
  if ((aDx < 0 || aCheckAllSides) && !CanWalkInDirection(DIRECTION_LEFT, aDx, aDy)) {
    return EFalse;
  }
  if ((aDx > 0 || aCheckAllSides) && !CanWalkInDirection(DIRECTION_RIGHT, aDx, aDy)) {
    return EFalse;
  }

  return ETrue;
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
  TBool ret;
  if (TestFlags(SFLAG_RENDER_SHADOW) && TestFlags(SFLAG_RENDER)) {
    if (mShadow.x1 == 0 && mShadow.x2 == 0 && mShadow.y1 == 0 && mShadow.y2 == 0) {
      ResetShadow();
    }
    TRect r = mShadow;
    r.Offset(x - aViewPort->mWorldX + aViewPort->mOffsetX, y - aViewPort->mWorldY + aViewPort->mOffsetY);
    if (aViewPort->GetRect().Overlaps(r)) {
      // render shadow beneath sprite
//    gDisplay.renderBitmap->SetColor(COLOR_SHADOW, 40, 40, 60);

      TFloat chord;
      for (TInt i = 0; i < r.Height(); i++) {
        chord = sqrt(i * (r.Height() - i)) * 2 * r.Width() / r.Height();
        gDisplay.renderBitmap->DrawFastHLine(aViewPort, r.x2 - aViewPort->mOffsetX - TInt(chord / 2),
          r.y1 - aViewPort->mOffsetY + i - 1, chord, COLOR_SHADOW);
      }
      ret = BAnimSprite::Render(aViewPort);
      ClearFlags(SFLAG_CLIPPED);
    } else {
      ret = BAnimSprite::Render(aViewPort);
    }
  } else {
    ret = BAnimSprite::Render(aViewPort);
  }

#ifdef DEBUG_MODE
  if (GGame::mDebug && !Clipped()) {
    // render sprite border if sprite is visible
    if (flags & SFLAG_RENDER) {
      gDisplay.renderBitmap->DrawRect(aViewPort, mRect, COLOR_WHITE);
      gDisplay.renderBitmap->DrawFastHLine(aViewPort, mRect.x1 - 5, mRect.y2, 10, COLOR_HEALTH);
      gDisplay.renderBitmap->DrawFastVLine(aViewPort, mRect.x1, mRect.y2 - 5, 10, COLOR_HEALTH);
    }
    // render collision rect
    if (flags & SFLAG_CHECK) {
      TRect r;
      GetRect(r);
      r.x1 -= aViewPort->mWorldX;
      r.x2 -= aViewPort->mWorldX;
      r.y1 -= aViewPort->mWorldY;
      r.y2 -= aViewPort->mWorldY;
//      gDisplay.SetColor(COLOR_HEALTH, 255, 0, 0);
      gDisplay.renderBitmap->DrawRect(aViewPort, r, COLOR_HEALTH);
    }
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

DIRECTION GAnchorSprite::RotateDirection(DIRECTION aDirection, TInt aRotateClockwiseCount) {
  aRotateClockwiseCount %= 4;
  if (aRotateClockwiseCount == 0) {
    return aDirection;
  } else {
    // recursive call
    aRotateClockwiseCount--;
    switch (aDirection) {
      case DIRECTION_UP:
        return GAnchorSprite::RotateDirection(DIRECTION_RIGHT, aRotateClockwiseCount);
      case DIRECTION_DOWN:
        return GAnchorSprite::RotateDirection(DIRECTION_LEFT, aRotateClockwiseCount);
      case DIRECTION_LEFT:
        return GAnchorSprite::RotateDirection(DIRECTION_UP, aRotateClockwiseCount);
      case DIRECTION_RIGHT:
        return GAnchorSprite::RotateDirection(DIRECTION_DOWN, aRotateClockwiseCount);
    }
  }
}

void GAnchorSprite::StartAnimationInDirection(ANIMSCRIPT* aScriptGroup[4], DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      StartAnimation(aScriptGroup[DIRECTION_UP]);
      break;
    case DIRECTION_DOWN:
      StartAnimation(aScriptGroup[DIRECTION_DOWN]);
      break;
    case DIRECTION_LEFT:
      StartAnimation(aScriptGroup[DIRECTION_LEFT]);
      break;
    case DIRECTION_RIGHT:
      StartAnimation(aScriptGroup[DIRECTION_RIGHT]);
      break;
    default:
      Panic("No animation direction\n");
      break;
  }
}

void GAnchorSprite::MoveInDirection(TFloat aSpeed, DIRECTION aDirection) {
  switch (aDirection) {
    case DIRECTION_UP:
      vy = -aSpeed;
      break;
    case DIRECTION_DOWN:
      vy = aSpeed;
      break;
    case DIRECTION_LEFT:
      vx = -aSpeed;
      break;
    case DIRECTION_RIGHT:
      vx = aSpeed;
      break;
    default:
      Panic("No move direction\n");
  }
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
  mAttackStrength = mBaseStrength + mLevel * (mBaseStrength / 5);
  mExperience = mBaseExperience + mLevel * (mBaseExperience / 5);
}

void GAnchorSprite::GetFloatRect(GFloatRect &aRect) {
  aRect.Set(x + cx + w / 2.0, y + cy - h, x + cx + w + w / 2.0, y + cy);
}

void GAnchorSprite::WriteToStream(BMemoryStream &aStream) {
  aStream.Write(&x, sizeof(x));
  aStream.Write(&y, sizeof(y));
  aStream.Write(&vx, sizeof(vx));
  aStream.Write(&vy, sizeof(vy));
  aStream.Write(&cx, sizeof(cx));
  aStream.Write(&cy, sizeof(cy));
  aStream.Write(&w, sizeof(w));
  aStream.Write(&h, sizeof(h));
  aStream.Write(&pri, sizeof(pri));
  aStream.Write(&flags, sizeof(flags));
  aStream.Write(&mDirection, sizeof(mDirection));
}

void GAnchorSprite::ReadFromStream(BMemoryStream &aStream) {
  aStream.Read(&x, sizeof(x));
  aStream.Read(&y, sizeof(y));
  aStream.Read(&vx, sizeof(vx));
  aStream.Read(&vy, sizeof(vy));
  aStream.Read(&cx, sizeof(cx));
  aStream.Read(&cy, sizeof(cy));
  aStream.Read(&w, sizeof(w));
  aStream.Read(&h, sizeof(h));
  aStream.Read(&pri, sizeof(pri));
  aStream.Read(&flags, sizeof(flags));
  aStream.Read(&mDirection, sizeof(mDirection));
}

void GAnchorSprite::Dump() {
  printf("\nSPRITE @ %p\n", this);
  printf("%-16.16s: %f(%f),%f(%f)\n", "x(vx),y(vy)", x, vx, y, vy);
  printf("%-16.16s: %d,%d,%d,%d\n", "cx,cy,w,h", cx, cy, w, h);
  printf("%-16.16s: %d\n", "pri", pri);
  printf("%-16.16s: %d\n", "flags", flags);
  printf("\n");
}
