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
  mSpriteSheet = gResourceManager.LoadSpriteSheet(CHARA_HERO_BMP_SPRITES);
  SetFlags(SFLAG_ANCHOR | SFLAG_CHECK | SFLAG_RENDER_SHADOW); // SFLAG_SORTY
}

TBool GPlayerSprite::Render(BViewPort *aViewPort) {
  TBool ret = GAnchorSprite::Render(aViewPort);
  if (GPlayer::mSwordCharge > 0.0 && mHitPoints > 0) {
    TRect r = TRect(mRect.x1 + 16, mRect.y2 - 41, mRect.x2 - 16, mRect.y2 - 40);
    TUint8 meterColor = GPlayer::mSwordCharge >= 2.0 ? COLOR_SHMOO_GREEN : COLOR_EXPERIENCE;
    gDisplay.renderBitmap->FillRect(aViewPort, r, COLOR_WHITE);
    gDisplay.renderBitmap->FillRect(aViewPort, r.x1, r.y1, r.x1 + MIN(TFloat(r.Width() - 1) * GPlayer::mSwordCharge, r.Width() - 1), r.y2, meterColor);
  }
  return ret;
}

