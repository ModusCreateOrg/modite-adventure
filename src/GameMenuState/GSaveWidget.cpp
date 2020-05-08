#include "GSaveWidget.h"

GSaveWidget::GSaveWidget(GGameState *aGameState) : GButtonWidget("Save Progress") {
  mGameState = aGameState;
  mHeight = 24;

}

GSaveWidget::~GSaveWidget() = default;

TInt GSaveWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight + 10;
}

void GSaveWidget::Select() {
  if (gGame->GetState() != GAME_STATE_GAME && gGame->GetState() != GAME_STATE_RESUME_GAME) {
    return;
  }

  // TODO: save gGameEngine state
  mGameState->SaveState();


  // Simulate start button press to exit menu
  gControls.dKeys |= BUTTON_START;

#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxSaveGame();
#endif
}
