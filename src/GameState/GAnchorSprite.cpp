#include <GameState/player/GPlayer.h>
#include <GameState/inventory/Items.h>
#include <GameState/status/GStatProcess.h>
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

TBool GAnchorSprite::IsFloorTile(TFloat aX, TFloat aY) {
  return mGameState->mGamePlayfield->IsFloor(aX, aY);
}

TBool GAnchorSprite::IsFloor(DIRECTION aDirection, TFloat aVx, TFloat aVy) {
  TRect r;
  GetRect(r);
  r.Offset(aVx, aVy);

  switch (aDirection) {
    case DIRECTION_UP:
      if (IsFloorTile(r.x1 + FLOOR_ADJUST_LEFT, r.y1) && IsFloorTile(r.x2 - FLOOR_ADJUST_RIGHT, r.y1)) {
        return ETrue;
      }
      break;
    case DIRECTION_SPELL:
    case DIRECTION_DOWN:
      if (IsFloorTile(r.x1 + FLOOR_ADJUST_LEFT, r.y2) && IsFloorTile(r.x2 - FLOOR_ADJUST_RIGHT, r.y2)) {
        return ETrue;
      }
      break;
    case DIRECTION_LEFT:
      if (IsFloorTile(r.x1, r.y1 + FLOOR_ADJUST_TOP) && IsFloorTile(r.x1, r.y2 - FLOOR_ADJUST_BOTTOM)) {
        return ETrue;
      }
      break;
    case DIRECTION_RIGHT:
      if (IsFloorTile(r.x2, r.y1 + FLOOR_ADJUST_TOP) && IsFloorTile(r.x2, r.y2 - FLOOR_ADJUST_BOTTOM)) {
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
  if ((flags & SFLAG_RENDER_SHADOW) && (flags & SFLAG_RENDER) && !Clipped()) {
    if (mShadow.x1 == 0 && mShadow.x2 == 0 && mShadow.y1 == 0 && mShadow.y2 == 0) {
      ResetShadow();
    }

    // render shadow beneath sprite
    gDisplay.renderBitmap->SetColor(COLOR_SHADOW, 40, 40, 60);

    TFloat screenX = x - aViewPort->mWorldX;
    TFloat screenY = y - aViewPort->mWorldY;
    for (TInt i = mShadow.y1; i < mShadow.y2; i++) {
      TFloat chord = sqrt(pow(mShadow.Height() / 2, 2) - pow(i - ((mShadow.y1 + mShadow.y2) / 2), 2)) * 2 * mShadow.Width() / mShadow.Height();
      gDisplay.renderBitmap->DrawFastHLine(aViewPort, screenX + mShadow.x1 + mShadow.Width() - TInt(chord / 2 + 1), screenY + i, chord, COLOR_SHADOW);
    }
  }

  TBool ret = BAnimSprite::Render(aViewPort);

#ifdef DEBUG_MODE
  if (GGame::mDebug && !Clipped()) {
    // render sprite border if sprite is visible
    if (flags & SFLAG_RENDER) {
      gDisplay.renderBitmap->DrawRect(aViewPort, mRect, COLOR_TEXT);
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

TBool GAnchorSprite::MaybeDamage(TBool aSpell) {
  if (!mInvulnerable) {
    TInt hitAmount = GPlayer::mHitStrength;
    ELEMENT hitElement = ELEMENT_NONE;
    if (aSpell) {
      if (GPlayer::mEquipped.mSpellbook) {
        switch (GPlayer::mEquipped.mSpellbook->mItemNumber) {
          case ITEM_BLUE_SPELLBOOK:
            hitElement = ELEMENT_WATER;
            break;
          case ITEM_RED_SPELLBOOK:
            hitElement = ELEMENT_FIRE;
            break;
          case ITEM_GREEN_SPELLBOOK:
            hitElement = ELEMENT_EARTH;
            break;
          case ITEM_YELLOW_SPELLBOOK:
            hitElement = ELEMENT_ENERGY;
            break;
          default:
            break;
        }
        if (hitElement && mElement) {
          hitAmount *= SPELLBOOK_MATRIX[mElement - 1][hitElement - 1];
        } else {
          hitAmount *= SPELL_HIT_BONUS;
        }
      }
    } else {
      // Random +/- 20% damage modifier
      hitAmount = (hitAmount * Random(80, 120)) / 100;
      if (GPlayer::mEquipped.mRing) {
        switch (GPlayer::mEquipped.mRing->mItemNumber) {
          case ITEM_BLUE_RING:
            hitElement = ELEMENT_WATER;
            break;
          case ITEM_RED_RING:
            hitElement = ELEMENT_FIRE;
            break;
          case ITEM_GREEN_RING:
            hitElement = ELEMENT_EARTH;
            break;
          case ITEM_YELLOW_RING:
            hitElement = ELEMENT_ENERGY;
            break;
          default:
            break;
        }
        if (hitElement && mElement) {
          hitAmount *= RING_MATRIX[mElement - 1][hitElement - 1];
        } else {
          hitAmount *= RING_HIT_BONUS;
        }
      }
    }
    mHitPoints -= hitAmount;
    auto *p = new GStatProcess(x + 68, y + 32, "%d", hitAmount);
    p->SetMessageType(STAT_ENEMY_HIT);
    mGameState->AddProcess(p);
    gSoundPlayer.SfxEnemyTakeDamage();

    return ETrue;
  }
  return EFalse;
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
