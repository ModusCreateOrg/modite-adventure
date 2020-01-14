#include "GOptionsWidget.h"
#include "GGameMenuContainer.h"

GOptionsWidget::GOptionsWidget(GGameMenuContainer *aContainer, TBool aExit) : GButtonWidget(aExit ? "Back" : "Options") {
  mContainer = aContainer;
  mExit = aExit;
  mHeight = 24;
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
