#include "GAnchorSprite.h"
#include <Display.h>

GAnchorSprite::GAnchorSprite(TInt aPri, TUint16 aBM, TUint16 aImg, TUint16 aType) :
  BAnimSprite(aPri, aBM, aImg, aType) {
  flags |= SFLAG_ANCHOR | SFLAG_SORTY;
  w = 64;
  h = 64;
}

GAnchorSprite::~GAnchorSprite() {
  //
}

TBool GAnchorSprite::Render(BViewPort *aViewPort) {
  BAnimSprite::Render(aViewPort);
  gDisplay.renderBitmap->SetColor(255, 255, 0, 255);
  gDisplay.renderBitmap->DrawRect(aViewPort, mRect, 255);
  return ETrue;
}

void GAnchorSprite::Collide(BSprite *aOther) {
  if (abs(aOther->y - y) < 6) {
    cType |= aOther->type;
  }
}