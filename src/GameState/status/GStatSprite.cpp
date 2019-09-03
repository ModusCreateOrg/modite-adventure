#include "Game.h"
#include "GStatSprite.h"

GStatSprite::GStatSprite(STAT_SIZE aSize, const char *aMessage)
    : mMessage(strdup(aMessage)), BSprite(0, 0, 0, STYPE_DEFAULT) {

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
  const TInt screenX = x - aViewPort->mWorldX, screenY = y - aViewPort->mWorldY;

    gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
    return gDisplay.renderBitmap->DrawString(
        aViewPort, mMessage, mFont, screenX, screenY, COLOR_SHMOO, -1, -6);
}
