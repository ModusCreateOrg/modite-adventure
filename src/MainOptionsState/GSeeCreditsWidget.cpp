#include "GSeeCreditsWidget.h"

GSeeCreditsWidget::GSeeCreditsWidget() : GButtonWidget("Credits") {
  mHeight = 16;
}

GSeeCreditsWidget::~GSeeCreditsWidget() = default;

TInt GSeeCreditsWidget::Render(TInt aX, TInt aY) {
  aY += mHeight;
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 0;
}

void GSeeCreditsWidget::Select() {
  gGame->SetState(GAME_STATE_VICTORY);
}
