#include "GSaveWidget.h"

GSaveWidget::GSaveWidget(GGameState *aGameState) : GButtonWidget("SAVE GAME") {
  mGameState = aGameState;
}

GSaveWidget::~GSaveWidget() {}

TInt GSaveWidget::Render(TInt aX, TInt aY) {
#ifndef __XTENSA__
  aY += 10;
#endif
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
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
