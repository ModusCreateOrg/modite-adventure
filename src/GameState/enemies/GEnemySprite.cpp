#include <Display.h>
#include "GResources.h"
#include "GEnemySprite.h"
#include "GPlayer.h"

GEnemySprite::GEnemySprite(GGameState *aGameState, TInt aPri, TUint16 aBM, TUint16 aImg, TUint16 aType) : GAnchorSprite(
        aGameState, aPri, aBM, aImg, aType) {
  SetCMask(STYPE_PLAYER | STYPE_PBULLET | STYPE_OBJECT);
  SetFlags(SFLAG_KNOCKBACK | SFLAG_CHECK | SFLAG_RENDER_SHADOW);

  mMeter = 1.0;
}

GEnemySprite::~GEnemySprite() {
  if (GPlayer::mClosestEnemy == this) {
    GPlayer::mClosestEnemy = ENull;
  }
  if (GPlayer::mTargeted == this) {
    GPlayer::mTargeted = ENull;
  }
}

TBool GEnemySprite::Render(BViewPort *aViewPort) {
  if (gControls.IsPressed(CONTROL_TARGET) && !GPlayer::mTargeted) {
    if (!GPlayer::mClosestEnemy ||
        DistanceTo(GPlayer::mSprite) < GPlayer::mClosestEnemy->DistanceTo(GPlayer::mSprite)) {
      GPlayer::mClosestEnemy = this;
    }
  }
  TBool ret = GAnchorSprite::Render(aViewPort);
  if ((this == GPlayer::mTargeted || mMeter < 1.0) && !Clipped()) {
    gDisplay.renderBitmap->DrawRect(aViewPort, MID(mRect.x1, mRect.x2) - 11, mRect.y2 - 41,
                                    MID(mRect.x1, mRect.x2) + 10, mRect.y2 - 39, COLOR_METER_OUTLINE);
    gDisplay.renderBitmap->DrawFastHLine(aViewPort, MID(mRect.x1, mRect.x2) - 10, mRect.y2 - 40, 20, COLOR_TEXT);
    if (mMeter > 0.0) {
      gDisplay.renderBitmap->DrawFastHLine(aViewPort, MID(mRect.x1, mRect.x2) - 10, mRect.y2 - 40,
                                           MAX(mMeter * 20, 1), COLOR_HEALTH2);
    }
  }
  if (this == GPlayer::mTargeted) {
    gDisplay.renderBitmap->DrawString(aViewPort, "v", gFont16x16, MID(mRect.x1, mRect.x2) - 8, mRect.y2 - 60, COLOR_SHMOO_RED, -1);
  }
  return ret;
}

