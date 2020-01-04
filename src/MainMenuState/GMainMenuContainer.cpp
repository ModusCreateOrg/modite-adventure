#include "GMainMenuContainer.h"
#include "GStartWidget.h"
#include "GResumeGameWidget.h"
#include "common/GButtonWidget.h"
#include "GMainMenuProcess.h"
#include "common/GSavedGameList.h"

GMainMenuContainer::GMainMenuContainer(TInt aX, TInt aY, GMainMenuProcess *aProcess) : GDialogWidget("Menu", aX, aY) {
  mProcess = aProcess;
  if (gSavedGameList.mNumSavedGames) {
    AddWidget((BWidget &) *new GResumeGameWidget());
  }
  AddWidget((BWidget &) *new GStartWidget());

  AddWidget((BWidget &) *new GButtonWidget("OPTIONS", GAME_STATE_MAIN_OPTIONS));
  AddWidget((BWidget &) *new GButtonWidget("QUIT", GAME_STATE_QUIT));
}

GMainMenuContainer::~GMainMenuContainer() {}

TBool GMainMenuContainer::OnNavigate(TUint16 bits) {
  GDialogWidget::OnNavigate(bits);
  mProcess->ResetTimer();
  return ETrue;
}
