#include "GResumeWidget.h"

GResumeWidget::GResumeWidget() : GButtonWidget("RESUME") {}

GResumeWidget::~GResumeWidget() {}

TInt GResumeWidget::Render(TInt aX, TInt aY) {
#ifdef __XTENSA__
  aY += 16;
#else
  aY += 20;
#endif
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GResumeWidget::Select() {
  // Simulate start button press to exit menu
  gControls.dKeys |= BUTTON_START;

#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuAccept();
#endif
}
