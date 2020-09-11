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
  if (mMeter >= 0 && (this == GPlayer::mTargeted || mMeter < 1.0) && !Clipped()) {
    gDisplay.renderBitmap->DrawRect(aViewPort, MID(mRect.x1, mRect.x2) - 11, mRect.y2 - 41,
                                    MID(mRect.x1, mRect.x2) + 10, mRect.y2 - 39, COLOR_METER_OUTLINE);
    gDisplay.renderBitmap->DrawFastHLine(aViewPort, MID(mRect.x1, mRect.x2) - 10, mRect.y2 - 40, 20, COLOR_TEXT);
    if (mMeter > 0.0) {
      gDisplay.renderBitmap->DrawFastHLine(aViewPort, MID(mRect.x1, mRect.x2) - 10, mRect.y2 - 40,
                                           MAX(mMeter * 20, 1), COLOR_HEALTH2);
    }
  }
  return ret;
}

void GEnemySprite::RenderAboveShadow(BViewPort *aViewPort) {
  if (this == GPlayer::mTargeted) {
    BBitmap *bm = gDisplay.renderBitmap;
    // render target indicator
    TRect r = mShadow;
    TFloat chord;
    r.Offset(x - aViewPort->mWorldX, y - aViewPort->mWorldY);
    for (TInt i = 0; i < r.Height(); i++) {
      chord = sqrt(i * (r.Height() - i)) * 2 * r.Width() / r.Height();
      bm->DrawFastHLine(aViewPort, r.x2 - TInt(chord / 2), r.y1 + i - 1, 2, COLOR_SHMOO_RED);
      bm->DrawFastHLine(aViewPort, r.x2 + TInt(chord / 2) - 1, r.y1 + i - 1, 2, COLOR_SHMOO_RED);
      if (i < 2 || i >= r.Height() - 2) {
        bm->DrawFastHLine(aViewPort, r.x2 - TInt(chord / 2),
                          r.y1 + i - 1, chord, COLOR_SHMOO_RED);
      }
    }
    r.Offset( TInt(r.Width() / 2), 0);
    bm->DrawLine(aViewPort, r.x1, r.y1, r.x2 - 1, r.y2 - 1, COLOR_SHMOO_RED);
    bm->DrawLine(aViewPort, r.x1, r.y1 + 1, r.x2 - 1, r.y2, COLOR_SHMOO_RED);
    bm->DrawLine(aViewPort, r.x1, r.y2 - 1, r.x2 - 1, r.y1, COLOR_SHMOO_RED);
    bm->DrawLine(aViewPort, r.x1, r.y2, r.x2 - 1, r.y1 + 1, COLOR_SHMOO_RED);
  }
}
