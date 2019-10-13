#include "GMainMenuContainer.h"
#include "GStartWidget.h"
#include "GMainMenuProcess.h"

GMainMenuContainer::GMainMenuContainer(TInt aX, TInt aY, GMainMenuProcess *aProcess) : GDialogWidget("Menu", aX, aY) {
  mProcess = aProcess;
  AddWidget((BWidget &) *new GStartWidget());
  AddWidget((BWidget &) *new GButtonWidget("OPTIONS", GAME_STATE_MAIN_OPTIONS));
  AddWidget((BWidget &) *new GButtonWidget("CREDITS", GAME_STATE_CREDITS));
}

GMainMenuContainer::~GMainMenuContainer() {}

TBool GMainMenuContainer::OnNavigate(TUint16 bits) {
  GDialogWidget::OnNavigate(bits);
  mProcess->ResetTimer();
  return ETrue;
}
