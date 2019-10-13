#include "GResetWidget.h"

GResetWidget::GResetWidget() : GButtonWidget("RESET", GAME_STATE_RESET_OPTIONS) {}

GResetWidget::~GResetWidget() {}

TInt GResetWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}
