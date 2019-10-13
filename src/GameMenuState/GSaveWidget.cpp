#include "GGameState.h"
#include "GSaveWidget.h"

GSaveWidget::GSaveWidget() : GButtonWidget("SAVE GAME") {}

GSaveWidget::~GSaveWidget() {}

TInt GSaveWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GSaveWidget::Select() {
  if (gGame->GetState() != GAME_STATE_GAME) {
    return;
  }

  // TODO: save gGameEngine state

  // Simulate start button press
  gControls.dKeys |= BUTTON_START;

#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxSaveGame();
#endif
}
