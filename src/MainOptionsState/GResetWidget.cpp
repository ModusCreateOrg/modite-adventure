#include "GResetWidget.h"

GResetWidget::GResetWidget() : GButtonWidget("Reset Game", GAME_STATE_RESET_OPTIONS) {
  mHeight = 14;
}

GResetWidget::~GResetWidget() = default;

TInt GResetWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 7;
}
