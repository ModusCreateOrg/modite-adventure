#include "GInventoryPlayfield.h"
#include "GPlayer.h"

GInventoryPlayfield::GInventoryPlayfield(GInventory *aInventory) : mInventory(aInventory) {
}

GInventoryPlayfield::~GInventoryPlayfield() {
}

void GInventoryPlayfield::Render() {
  BBitmap *bm = gDisplay.renderBitmap;
  bm->Clear(COLOR_TEXT_BG);
}

