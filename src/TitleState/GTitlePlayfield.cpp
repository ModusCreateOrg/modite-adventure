#include "GTitlePlayfield.h"

GTitlePlayfield::GTitlePlayfield() {
  gResourceManager.LoadBitmap(TITLE_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBackground = gResourceManager.GetBitmap(BKG_SLOT);
  gDisplay.SetPalette(mBackground);
}

GTitlePlayfield::~GTitlePlayfield() {
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
}

void GTitlePlayfield::Render() {
  gDisplay.renderBitmap->CopyPixels(mBackground);
}
