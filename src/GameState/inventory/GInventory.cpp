#include "GInventory.h"
#include "GInventoryPlayfield.h"
#include "GResources.h"
#include "Items.h"
#include "GInventoryProcess.h"

static const TUint16 ITEM_LAYOUT[4][4] = {
  { ITEM_RED_POTION1, ITEM_RED_POTION2, ITEM_BLUE_POTION1, ITEM_BLUE_POTION2 },
  { ITEM_WATER_SPELLBOOK, ITEM_FIRE_SPELLBOOK, ITEM_EARTH_SPELLBOOK, ITEM_ENERGY_SPELLBOOK },
  { ITEM_WATER_AMULET, ITEM_FIRE_AMULET, ITEM_EARTH_AMULET, ITEM_ENERGY_AMULET },
  { ITEM_WATER_RING, ITEM_FIRE_RING, ITEM_EARTH_RING, ITEM_ENERGY_RING }
};

GInventory::GInventory(BViewPort *aViewPort) : BGameEngine(aViewPort) {
  mPlayfield = new GInventoryPlayfield(this, aViewPort);
  mProcess = new GInventoryProcess(this);
  AddProcess(mProcess);

  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_WHITE, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_METER_OUTLINE, 64, 64, 64);
}

GInventory::~GInventory() = default;

GInventoryItem *GInventory::SelectedItem() {
  return mProcess->mSelectedItem;
}

TInt16 GInventory::ItemNumber() {
  return mProcess->mItemNumber;
}

TInt16 GInventory::ItemLayout(TUint8 x, TUint8 y) {
  return ITEM_LAYOUT[x][y];
}

const TUint16 *GInventory::ItemLayout(TUint8 x) {
  return ITEM_LAYOUT[x];
}

TInt GInventory::CurrentRow() {
  return mProcess->mCurrentRow;
}

TInt GInventory::CurrentColumn() {
  return mProcess->mCurrentColumn;
}

void GInventory::PostRender() {
  if (gControls.WasPressed(BUTTONR)) {
    gGame->ToggleInventory();
  }
}
