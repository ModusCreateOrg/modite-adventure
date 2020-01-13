#include "GOptionsWidget.h"
#include "GGameMenuContainer.h"

GOptionsWidget::GOptionsWidget(GGameMenuContainer *aContainer, TBool aExit) : GButtonWidget(aExit ? "Exit Options" : "Options") {
  mContainer = aContainer;
  mExit = aExit;
}

GOptionsWidget::~GOptionsWidget() {}

TInt GOptionsWidget::Render(TInt aX, TInt aY) {
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight + 10;
}

void GOptionsWidget::Select() {
  if (mExit) {
    mContainer->State(GAME_MENU_PAUSE_STATE);
  } else {
    mContainer->State(GAME_MENU_OPTIONS_STATE);
  }
  GButtonWidget::Select();
}
