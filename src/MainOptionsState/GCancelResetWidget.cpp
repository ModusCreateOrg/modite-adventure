#include "GCancelResetWidget.h"

GCancelResetWidget::GCancelResetWidget() : GButtonWidget("CANCEL", GAME_STATE_MAIN_OPTIONS) {}

GCancelResetWidget::~GCancelResetWidget() {}

TInt GCancelResetWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight << 1;
}

void GCancelResetWidget::Select() {
  gGame->SetState(GAME_STATE_MAIN_OPTIONS);
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuCancel();
#endif
}
