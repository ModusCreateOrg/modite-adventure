#include "GExitWidget.h"

GExitWidget::GExitWidget() : GButtonWidget("Back") {
  mHeight = 24;
}

GExitWidget::~GExitWidget() = default;

TInt GExitWidget::Render(TInt aX, TInt aY) {
  aY += mHeight;
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 0;
}

void GExitWidget::Select() {
  gGame->SetState(GAME_STATE_MAIN_MENU);
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuOut();
#endif
}
