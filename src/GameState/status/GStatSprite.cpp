#include "Game.h"
#include "GStatSprite.h"

GStatSprite::GStatSprite(TInt aSize, const char *aMessage, TInt aImage)
    : mMessage(strdup(aMessage)), BSprite(0, ENVIRONMENT_SLOT, 0, STYPE_DEFAULT) {

  SetFlags(SFLAG_BELOW);
  mImageNumber = aImage;
  mMessageType = STAT_INFO;

  switch (aSize) {
    case STAT_SIZE_8x8:
      mFont = gFont8x8;
      break;
    case STAT_SIZE_16x16:
      mFont = gFont16x16;
      break;
    default:
      Panic("GStatSprite: invalid STAT_SIZE %d\n", aSize);
  }
}

GStatSprite::~GStatSprite() {
  delete[] mMessage;
}

TBool GStatSprite::Render(BViewPort *aViewPort) {
  TInt screenX = x - aViewPort->mWorldX, screenY = y - aViewPort->mWorldY;

  if (mImageNumber) {
    y -= 8;
    BSprite::Render(aViewPort);
    screenX += gResourceManager.BitmapWidth(mBitmapSlot);
    y += 8;
  }

  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  switch (mMessageType) {
    case STAT_PLAYER_HIT:
      return gDisplay.renderBitmap->DrawString(aViewPort, mMessage, mFont, screenX, screenY, COLOR_SHMOO_RED, COLOR_TEXT_TRANSPARENT, -6);
    case STAT_EXPERIENCE:
    case STAT_HEAL:
      return gDisplay.renderBitmap->DrawString(aViewPort, mMessage, mFont, screenX, screenY, COLOR_SHMOO_GREEN, COLOR_TEXT_TRANSPARENT, -6);
    case STAT_ENEMY_HIT:
    case STAT_INFO:
    default:
      return gDisplay.renderBitmap->DrawString(aViewPort, mMessage, mFont, screenX, screenY, COLOR_SHMOO, COLOR_TEXT_TRANSPARENT, -6);
  }
}
