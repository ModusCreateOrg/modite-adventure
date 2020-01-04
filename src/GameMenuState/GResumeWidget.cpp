#include "GResumeWidget.h"

GResumeWidget::GResumeWidget() : GButtonWidget("RESUME") {}

GResumeWidget::~GResumeWidget() {}

TInt GResumeWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GResumeWidget::Select() {
  // Simulate start button press to exit menu
  gControls.dKeys |= BUTTON_START;

#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuIn();
#endif
}
