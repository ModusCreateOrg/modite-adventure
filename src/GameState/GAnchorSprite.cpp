#include "Game.h"
#include "GAnchorSprite.h"

GAnchorSprite::GAnchorSprite(TInt aPri, TUint16 aBM, TUint16 aImg, TUint16 aType) :
  BAnimSprite(aPri, aBM, aImg, aType) {
  flags |= SFLAG_ANCHOR | SFLAG_SORTY;
  w = 64;
  h = 64;
  mHitPoints = 5;
}

GAnchorSprite::~GAnchorSprite() {
  //
}

TBool GAnchorSprite::Render(BViewPort *aViewPort) {
  BAnimSprite::Render(aViewPort);

#ifdef DEBUG_MODE
  if (gGame->mDebug) {
    gDisplay.renderBitmap->SetColor(255, 255, 0, 255);
    gDisplay.renderBitmap->DrawRect(aViewPort, mRect, 255);
    // render collision rect
    TRect r(mRect.x1 + 16, mRect.y1 + 32, mRect.x2 - 16, mRect.y2);
    gDisplay.renderBitmap->SetColor(254, 255, 0, 0);
    gDisplay.renderBitmap->DrawRect(aViewPort, r, 254);
  }
#endif

  return ETrue;
}

void GAnchorSprite::Collide(BSprite *aOther) {
  if (abs(TInt(aOther->y - y)) < 6) {
    mCollided = (GAnchorSprite*)aOther;
    cType |= aOther->type;
  }
}
