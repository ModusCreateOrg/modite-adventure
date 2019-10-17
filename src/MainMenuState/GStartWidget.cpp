#include "GStartWidget.h"

GStartWidget::GStartWidget() : GButtonWidget("START", GAME_STATE_GAME) {
  if (gOptions->gameProgress.savedState) {
    mText = (char*)"CONTINUE";
  }
}

GStartWidget::~GStartWidget() {}
