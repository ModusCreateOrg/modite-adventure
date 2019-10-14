#include "GInventoryPlayfield.h"
#include "GPlayer.h"

GInventoryPlayfield::GInventoryPlayfield(GInventory *aInventory) : mInventory(aInventory) {
}

GInventoryPlayfield::~GInventoryPlayfield() {
}

void GInventoryPlayfield::Render() {
  gDisplay.renderBitmap->Clear(0);
}

