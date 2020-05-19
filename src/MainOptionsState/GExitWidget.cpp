#include "GExitWidget.h"

GExitWidget::GExitWidget() : GButtonWidget("Back") {
  mHeight = 20;
}

GExitWidget::~GExitWidget() {}

TInt GExitWidget::Render(TInt aX, TInt aY) {
#ifdef __XTENSA__
  aY += 16;
#else
  aY += 18;
#endif
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GExitWidget::Select() {
  gGame->SetState(GAME_STATE_MAIN_MENU);
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuOut();
#endif
}
