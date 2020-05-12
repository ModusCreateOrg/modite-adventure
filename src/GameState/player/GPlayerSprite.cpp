#include <Display.h>
#include <BResourceManager.h>
#include "GResources.h"
#include "GPlayerSprite.h"
#include "GPlayer.h"

GPlayerSprite::GPlayerSprite(GGameState *aGameState) : GAnchorSprite(
  aGameState, PLAYER_PRIORITY, PLAYER_SLOT) {
  Name("PLAYER");
  type = STYPE_PLAYER;
  SetCMask(STYPE_ENEMY | STYPE_EBULLET | STYPE_OBJECT); // collide with enemy, enemy attacks, and environment
  w = 24;
  h = 16;
  cx = 8;
  cy = 0;
  mSwordCharge = -1;
  mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_HERO_BMP_SPRITES);
  SetFlags(SFLAG_ANCHOR | SFLAG_CHECK); // SFLAG_SORTY
}

TBool GPlayerSprite::Render(BViewPort *aViewPort) {
  TBool ret = GAnchorSprite::Render(aViewPort);
  if (mSwordCharge > 0.0 && mHitPoints > 0) {
    TRect r = TRect(mRect.x1 + 16, mRect.y2 - 41, mRect.x2 - 16, mRect.y2 - 40);
    gDisplay.renderBitmap->FillRect(aViewPort, r, COLOR_WHITE);
    gDisplay.renderBitmap->DrawRect(aViewPort, r.x1 - 1, r.y1 - 1, r.x2 + 1, r.y2 + 1, COLOR_METER_OUTLINE);
    if (mSwordCharge >= 2.0) {
      gDisplay.renderBitmap->FillRect(aViewPort, r, COLOR_SHMOO_GREEN);
    } else {
      gDisplay.renderBitmap->FillRect(aViewPort, r.x1, r.y1, r.x1 + MIN(TFloat(r.Width() - 1) * mSwordCharge, r.Width() - 1), r.y2, COLOR_EXPERIENCE);
    }
  }
  return ret;
}

