#include <Display.h>
#include "GResources.h"
#include "GEnemySprite.h"

GEnemySprite::GEnemySprite(GGameState *aGameState, TInt aPri, TUint16 aBM, TUint16 aImg, TUint16 aType) : GAnchorSprite(
        aGameState, aPri, aBM, aImg, aType) {}

TBool GEnemySprite::Render(BViewPort *aViewPort) {
  TBool ret = GAnchorSprite::Render(aViewPort);
  if (mHitPoints < mMaxHitPoints && mMaxHitPoints > 0 && !Clipped()) {
    gDisplay.renderBitmap->DrawFastHLine(aViewPort, (mRect.x1 + mRect.x2) / 2 - 10, mRect.y2 - 40, 20, COLOR_TEXT);
    if (mHitPoints > 0) {
      gDisplay.renderBitmap->DrawFastHLine(aViewPort, (mRect.x1 + mRect.x2) / 2 - 10, mRect.y2 - 40,
                                           mHitPoints * 20 / mMaxHitPoints + 1, COLOR_HEALTH);
    }
  }
  return ret;
}

