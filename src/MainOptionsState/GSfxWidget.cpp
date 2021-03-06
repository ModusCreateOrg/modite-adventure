#include "GSfxWidget.h"
#include "Game.h"

static const TRange sfx_options = {
  0, 8, 1
};

GSfxWidget::GSfxWidget() : GSoundSliderWidget("SFX", &sfx_options, COLOR_TEXT, COLOR_TEXT_BG) {
  mHeight = 11;
}

GSfxWidget::~GSfxWidget() = default;

TInt GSfxWidget::Render(TInt aX, TInt aY) {
  mSelectedValue = gOptions->sfx / 0.125;
  return GSoundSliderWidget::Render(aX, aY);
}

void GSfxWidget::Select(TInt aVal) {
  gOptions->sfx = aVal * 0.125;
  gOptions->muted = EFalse;
  gOptions->Save();

#ifdef ENABLE_AUDIO
  gSoundPlayer.SetEffectsVolume(gOptions->sfx);
  gSoundPlayer.MuteMusic(gOptions->muted);
  gSoundPlayer.SfxOptionSelect();
#endif
}
