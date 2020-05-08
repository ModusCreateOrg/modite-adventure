#include "GExitDungeonWidget.h"

GExitDungeonWidget::GExitDungeonWidget() : GButtonWidget("Leave Dungeon") {}

GExitDungeonWidget::~GExitDungeonWidget() = default;

TInt GExitDungeonWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight + 10;
}

void GExitDungeonWidget::Select() {
  // Simulate start button press to exit menu
  gControls.dKeys |= BUTTON_START;

  ((GGameState *) gGameEngine)->TryAgain(ETrue);
  GButtonWidget::Select();
}
