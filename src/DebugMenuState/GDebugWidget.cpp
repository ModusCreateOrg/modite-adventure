#include "GDebugWidget.h"

GDebugWidget::GDebugWidget() : GButtonWidget("") {}

GDebugWidget::~GDebugWidget() {}

TInt GDebugWidget::Render(TInt aX, TInt aY) {
  aY += 20;
  mText = (char *)(GGame::mDebug ? "Hide Debug Info" :"Show Debug Info" );
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight;
}

void GDebugWidget::Select() {
  // Simulate start button press to exit menu
  // gControls.dKeys |= CONTROL_DEBUG;
  GGame::mDebug = !GGame::mDebug;
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuIn();
#endif
}
