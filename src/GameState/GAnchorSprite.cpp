#include "Game.h"
#include "GAnchorSprite.h"

GAnchorSprite::GAnchorSprite(TInt aPri, TUint16 aBM, TUint16 aImg, TUint16 aType) :
  BAnimSprite(aPri, aBM, aImg, aType) {
  flags |= SFLAG_ANCHOR | SFLAG_SORTY;
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
  if (GGame::mDebug) {
    gDisplay.renderBitmap->SetColor(255, 255, 0, 255);
    gDisplay.renderBitmap->DrawRect(aViewPort, mRect, 255);
    // render collision rect
//    TRect r(mRect.x1 + 16, mRect.y1 + 32, mRect.x2 - 16, mRect.y2);
    TRect r(mRect.x1 + w/2, mRect.y1 + h, mRect.x2 - w/2, mRect.y2);
    gDisplay.renderBitmap->SetColor(254, 255, 0, 0);
    gDisplay.renderBitmap->DrawRect(aViewPort, r, 254);

    gDisplay.SetColor(253, 255,0,0);
    gDisplay.renderBitmap->DrawFastHLine(aViewPort, x-5, y, 10, 253);
    gDisplay.renderBitmap->DrawFastVLine(aViewPort, x, y-5, 10, 253);
  }
#endif

  return ret;
}

void GAnchorSprite::Collide(BSprite *aOther) {
  GAnchorSprite *s = (GAnchorSprite *)aOther;
//  printf("%s collides with %s %x %x\n", mName, s->mName, type, s->type);
  if (abs(TInt(aOther->y - y)) < COLLISION_DELTA_Y) {
//    printf("abs() %d\n", TInt(abs(aOther->y - y)));
    mCollided = s;
    s->mCollided = this;
    cType |= aOther->type;
    aOther->cType |= type;
  }
}
