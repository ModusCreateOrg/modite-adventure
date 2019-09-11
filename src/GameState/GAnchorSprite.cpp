#include "Game.h"
#include "GAnchorSprite.h"

GAnchorSprite::GAnchorSprite(
    TInt aPri, TUint16 aBM, TUint16 aImg, TUint16 aType)
    : BAnimSprite(aPri, aBM, aImg, aType) {
  flags |= SFLAG_ANCHOR; //  | SFLAG_SORTY;
  w = 64;
  h = 64;
  mLevel = 1;
  mExperience = 0;
  mHitPoints = 5;
  mStrength = 10;
  mDexterity = 10;
  mGold = 0;
  strcpy(mName, "unnamed");
  mInvulnerable = EFalse;
}

GAnchorSprite::~GAnchorSprite() {
  //
}

TBool GAnchorSprite::Render(BViewPort *aViewPort) {
  TBool ret = BAnimSprite::Render(aViewPort);

#ifdef DEBUG_MODE
  if (GGame::mDebug && !Clipped()) {
    gDisplay.renderBitmap->SetColor(255, 255, 0, 255);
    gDisplay.renderBitmap->DrawRect(aViewPort, mRect, 255);
    // render collision rect
    TRect r;
    GetRect(r);
    r.x1 -= aViewPort->mWorldX;
    r.x2 -= aViewPort->mWorldX;
    r.y1 -= aViewPort->mWorldY;
    r.y2 -= aViewPort->mWorldY;
    gDisplay.renderBitmap->SetColor(254, 255, 0, 0);
    gDisplay.renderBitmap->DrawRect(aViewPort, r, 254);

    gDisplay.SetColor(253, 255, 0, 0);
    gDisplay.renderBitmap->DrawFastHLine(
        aViewPort, mRect.x1 - 5, mRect.y2, 10, 253);
    gDisplay.renderBitmap->DrawFastVLine(
        aViewPort, mRect.x1, mRect.y2 - 5, 10, 253);
  }
#endif

  return ret;
}

void GAnchorSprite::Collide(BSprite *aOther) {
  auto *s = (GAnchorSprite *)aOther;
  mCollided = s;
  s->mCollided = this;
  cType |= aOther->type;
  aOther->cType |= type;
}

void GAnchorSprite::Nudge() {
  TRect myRect, hisRect;
  GetRect(myRect);
  mCollided->GetRect(hisRect);
  if (myRect.Overlaps(hisRect)) {
    printf("before: %.2f,%.2f\n", x, y);

    myRect.Dump("myRect");
    hisRect.Dump("hisRect");
    if (myRect.x2 >= hisRect.x1 &&
        myRect.x1 > hisRect.x2) { // my right edge right of his left edge?
      x -= hisRect.x1 - myRect.x2;
    }
    else if (myRect.x1 <= hisRect.x2 && myRect.x2 < hisRect.x1) {
      x += hisRect.x2 - myRect.x1;
    }

    if (myRect.y2 >= hisRect.y1 && myRect.y1 > hisRect.y2) { // my bottom edge below his top edge?
      y -= hisRect.y1 - myRect.y2 + 4;
    }
    else if (myRect.y1 <= hisRect.y2) { // my top is above his bottom edge?
//      y -= hisRect.y2 - myRect.y1 + 4;
      y = hisRect.y2 + 36;
    }
    printf("after: %.2f,%.2f\n\n\n", x, y);
  }
}
