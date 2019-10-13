#include "GResetOptionsWidget.h"

GResetOptionsWidget::GResetOptionsWidget() : GButtonWidget("OPTIONS", GAME_STATE_MAIN_OPTIONS) {}

GResetOptionsWidget::~GResetOptionsWidget() {}

TInt GResetOptionsWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight << 1;
}

void GResetOptionsWidget::Select() {
  // Reset options
  gOptions->Reset();

  // Go Back
  gGame->SetState(GAME_STATE_MAIN_OPTIONS);

#ifdef ENABLE_AUDIO
  // Play notification sound
  gSoundPlayer.SfxMenuAccept();

  // Make sure we hear the reset in music volume immediately
  gSoundPlayer.SetMusicVolume(gOptions->music);
  gSoundPlayer.SetEffectsVolume(gOptions->sfx);
#endif
}
