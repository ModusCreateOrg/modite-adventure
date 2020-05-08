#include "GStartWidget.h"

GStartWidget::GStartWidget() : GButtonWidget("New Game", GAME_STATE_GAME) {
//  if (gOptions->gameProgress.savedState) {
//    mText = (char*)"CONTINUE";
//  }
}

GStartWidget::~GStartWidget() = default;
