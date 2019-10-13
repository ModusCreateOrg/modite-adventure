#include "GResetAllWidget.h"

GResetAllWidget::GResetAllWidget() : GButtonWidget("EVERYTHING", GAME_STATE_MAIN_OPTIONS) {}

GResetAllWidget::~GResetAllWidget() {}

TInt GResetAllWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight << 1;
}

void GResetAllWidget::Select() {
  gOptions->Reset();

  // Go Back
  gGame->SetState(GAME_STATE_MAIN_OPTIONS);

  // TODO: @jaygarcia
#ifdef ENABLE_AUDIO
  // Play notification sound
  gSoundPlayer.SfxMenuAccept();

  // Make sure we hear the reset in music volume immediately
  gSoundPlayer.SetMusicVolume(gOptions->music);
  gSoundPlayer.SetEffectsVolume(gOptions->sfx);
#endif
}
