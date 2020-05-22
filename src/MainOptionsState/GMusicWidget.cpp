#include "GMusicWidget.h"
#include "Game.h"

static const TRange music_options = {
  0, 8, 1
};

GMusicWidget::GMusicWidget() : GSoundSliderWidget("Music", &music_options, COLOR_TEXT, COLOR_TEXT_BG) {
  mHeight = 20;
}

GMusicWidget::~GMusicWidget() = default;

TInt GMusicWidget::Render(TInt aX, TInt aY) {
  mSelectedValue = gOptions->music / 0.125;
  return GSoundSliderWidget::Render(aX, aY);
}

void GMusicWidget::Select(TInt aVal) {
  gOptions->music = aVal * 0.125;
  gOptions->muted = EFalse;
  gOptions->Save();
#ifdef ENABLE_AUDIO
  gSoundPlayer.SetMusicVolume(gOptions->music);
  gSoundPlayer.MuteMusic(gOptions->muted);
#endif
}
