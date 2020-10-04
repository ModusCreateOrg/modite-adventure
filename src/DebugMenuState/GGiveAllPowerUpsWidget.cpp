#include "Game.h"
#include "GItemWidget.h"
#include "GPlayer.h"
#include "Items.h"
#include "GStatProcess.h"
#include "GGiveAllPowerUpsWidget.h"


GGiveAllPowerUpsWidget::GGiveAllPowerUpsWidget() : GButtonWidget("Give all Power Ups") {
  mHeight = 25;
}

TInt GGiveAllPowerUpsWidget::Render(TInt aX, TInt aY) {
  aY += mHeight;
  GButtonWidget::Render(aX, aY);
  return mHeight;
}

void GGiveAllPowerUpsWidget::Select() {
  // Pick up ONE item of item type 1 through 4
  for (TInt8 i = 1; i < 5; i++) {
    if (!GPlayer::mInventoryList.FindItem(i)) {
      GPlayer::mInventoryList.PickupItem(i);
    }
  }

  // ** // Skip Items 5 (silver key) and 6 (gold key)

  // Pick up ONE item of item type 7 through 18
  for (TInt8 i = 7; i < 18; i++) {
    if (!GPlayer::mInventoryList.FindItem(i)) {
      GPlayer::mInventoryList.PickupItem(i);
    }
  }

  // Pick up 20x items of item types 18 - 21 (life and mana potions)
  // If you spam this widget, you'll rack up more mana and life potions
  for (TInt8 i = 18; i < 22; i++) {
    for (TInt8 j = 0; j < 20; ++j) {
      GPlayer::mInventoryList.PickupItem(i);
    }
  }

  GPlayer::mInventoryList.Dump();


#ifdef ENABLE_AUDIO
  gSoundPlayer.TriggerSfx(SFX_ITEM_PICKUP_GENERIC_WAV, 2);
#endif
}