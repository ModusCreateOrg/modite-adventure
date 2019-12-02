#include "GDialogWidget.h"

GDialogWidget::GDialogWidget(char *aTitle, TInt aX, TInt aY) : BDialogWidget(aTitle, aX, aY) {
}

GDialogWidget::GDialogWidget(const char *aTitle, TInt aX, TInt aY) : BDialogWidget(aTitle, aX, aY) {
}

GDialogWidget::~GDialogWidget() {

}

TBool GDialogWidget::OnNavigate(TUint16 bits) {

#ifdef ENABLE_AUDIO
  //TODO: @Jgarcia
  if (bits & JOYUP) {
    gSoundPlayer.SfxMenuNavDown();
  } else if (bits & JOYDOWN) {
    gSoundPlayer.SfxMenuNavUp();
  }
#endif
  return ETrue;
}

