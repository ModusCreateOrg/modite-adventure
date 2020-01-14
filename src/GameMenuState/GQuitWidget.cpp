#include "GQuitWidget.h"

GQuitWidget::GQuitWidget() : GButtonWidget("Give up", GAME_STATE_MAIN_MENU) {}

GQuitWidget::~GQuitWidget() {}

TInt GQuitWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight;
}

void GQuitWidget::Select() {
  // Simulate start button press to exit menu
  gControls.dKeys |= BUTTON_START;

  GButtonWidget::Select();
}
