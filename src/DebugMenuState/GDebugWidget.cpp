#include "GDebugWidget.h"

GDebugWidget::GDebugWidget() : GButtonWidget("") {}

GDebugWidget::~GDebugWidget() {}

TInt GDebugWidget::Render(TInt aX, TInt aY) {
  aY += 20;
  mText = (char *)(GGame::mDebug ? "HIDE DEBUG INFO" :"SHOW DEBUG INFO" );
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GDebugWidget::Select() {
  // Simulate start button press to exit menu
  // gControls.dKeys |= BUTTON_MENU;
  GGame::mDebug = !GGame::mDebug;
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuAccept();
#endif
}
