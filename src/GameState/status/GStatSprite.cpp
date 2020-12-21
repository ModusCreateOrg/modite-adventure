#include "Game.h"
#include "GStatSprite.h"
#include <stdlib.h>
GStatSprite::GStatSprite(STAT_TYPE aType, const char *aMessage, TInt aImage)
    : mMessageType(aType), mMessage(strdup(aMessage)), BSprite(0, ENVIRONMENT_SLOT, 0, STYPE_DEFAULT) {

  SetFlags(SFLAG_BELOW);
  mImageNumber = aImage;

//  printf("GStatSprite Allocating %p (mImageNumber %i)\n", this, mImageNumber);

}

GStatSprite::~GStatSprite() {
//  printf("GStatSprite Freeing %p\n", this);
  free(mMessage);
}

TBool GStatSprite::Render(BViewPort *aViewPort) {
  TInt screenX = x - aViewPort->mWorldX, screenY = y - aViewPort->mWorldY;

  if (mImageNumber) {
    y -= 8;
    BSprite::Render(aViewPort);
    screenX += gResourceManager.BitmapWidth(mBitmapSlot);
    y += 8;
  }

  TUint8 color;
  switch (mMessageType) {
    case STAT_PLAYER_HIT:
      color = COLOR_SHMOO_RED;
      break;
    case STAT_EXPERIENCE:
    case STAT_HEAL:
      color = COLOR_SHMOO_GREEN;
      break;
    case STAT_MANA:
      color = COLOR_MAGIC;
      break;
    case STAT_ENEMY_HIT:
    case STAT_INFO:
    default:
      color = COLOR_SHMOO;
      break;
  }
  return gDisplay.renderBitmap->DrawStringShadow(aViewPort, mMessage, gFont16x16, screenX, screenY, color, COLOR_TEXT_SHADOW, COLOR_TEXT_TRANSPARENT, -6);
}
