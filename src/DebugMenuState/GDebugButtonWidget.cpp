#include "GDebugButtonWidget.h"
#include "GDebugMenuContainer.h"

GDebugButtonWidget::GDebugButtonWidget(const char *mText, TInt aState, GDebugMenuContainer *aContainer) : GButtonWidget(mText, aState) {
  mContainer = aContainer;
}

GDebugButtonWidget::~GDebugButtonWidget() {}

TInt GDebugButtonWidget::Render(TInt aX, TInt aY) {
  aY += 20;
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight + 4;
}

void GDebugButtonWidget::Select() {
  mContainer->SetState(mState);
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuIn();
#endif
}
