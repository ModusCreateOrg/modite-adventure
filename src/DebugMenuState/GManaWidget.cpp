#include "Game.h"
#include "GManaWidget.h"
#include "GPlayer.h"

static const TRange health_options = {
  0, 100, 25
};

GManaWidget::GManaWidget() : GSoundSliderWidget("MANA", &health_options, COLOR_TEXT, COLOR_TEXT_BG) {
  mHeight = 25;
}

GManaWidget::~GManaWidget() {}

TInt GManaWidget::RenderTitle(TInt aX, TInt aY, TBool aActive) {
  aY += mHeight;
  return GSoundSliderWidget::RenderTitle(aX, aY, aActive);
}

TInt GManaWidget::Render(TInt aX, TInt aY) {
  aY += mHeight;
  mSelectedValue = GPlayer::mManaPotion;
  GSoundSliderWidget::Render(aX, aY);
  return 4;
}

void GManaWidget::Select(TInt aVal) {
  GPlayer::mManaPotion = aVal;
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxOptionSelect();
#endif
}
