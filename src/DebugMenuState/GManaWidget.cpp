#include "Game.h"
#include "GManaWidget.h"
#include "GPlayer.h"

GManaWidget::GManaWidget() : GButtonWidget("Fill Mana") {
  mHeight = 20;
}

TInt GManaWidget::Render(TInt aX, TInt aY) {
  aY += mHeight;
  GButtonWidget::Render(aX, aY);
  return mHeight; //gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight;
}

void GManaWidget::Select() {
  GPlayer::mManaPotion = GPlayer::mMaxMana;
#ifdef ENABLE_AUDIO
  gSoundPlayer.TriggerSfx(SFX_PLAYER_QUAFF_HEALTH_POTION_WAV, 2);
#endif
}
