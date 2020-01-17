#include "GResumeGameWidget.h"
#include "common/GSavedGameList.h"

GResumeGameWidget::GResumeGameWidget() : GButtonWidget("Resume Game", -1) {
  gSavedGameList.LoadSavedGameList();
}

GResumeGameWidget::~GResumeGameWidget() {}

void GResumeGameWidget::Select() {
  GButtonWidget::Select();
  char saveGameName[256];
  gSavedGameList.First()->GameName(saveGameName);
  gGame->StartGame(saveGameName);
}
