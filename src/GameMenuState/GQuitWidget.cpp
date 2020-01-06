#include "GQuitWidget.h"

GQuitWidget::GQuitWidget() : GButtonWidget("QUIT GAME", GAME_STATE_MAIN_MENU) {}

GQuitWidget::~GQuitWidget() {}

TInt GQuitWidget::Render(TInt aX, TInt aY) {
#ifdef __XTENSA__
  aY += 16;
#else
  aY += 20;
#endif
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GQuitWidget::Select() {
  // Simulate start button press to exit menu
  gControls.dKeys |= BUTTON_START;

  GButtonWidget::Select();
}
