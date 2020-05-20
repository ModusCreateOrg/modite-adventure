#include "GDialogWidget.h"
#include "Game.h"

const TUint REPEAT_INPUT_DELAY = 0.5 * FRAMES_PER_SECOND;
const TUint REPEAT_INPUT_INTERVAL = 0.15 * FRAMES_PER_SECOND;

GDialogWidget::GDialogWidget(char *aTitle, TInt aX, TInt aY) : BDialogWidget(aTitle, aX, aY) {
  mTimer = 0;
}

GDialogWidget::GDialogWidget(const char *aTitle, TInt aX, TInt aY) : BDialogWidget(aTitle, aX, aY) {
  mTimer = 0;
}

GDialogWidget::~GDialogWidget() = default;

TBool GDialogWidget::OnNavigate(TUint16 bits) {

#ifdef ENABLE_AUDIO
  if (bits & JOYUP) {
    gSoundPlayer.SfxMenuNavDown();
  } else if (bits & JOYDOWN) {
    gSoundPlayer.SfxMenuNavUp();
  }
#endif
  return ETrue;
}

void GDialogWidget::Run() {
  mList.Run();

  if (!gControls.IsPressed(JOYUP | JOYDOWN)) {
    mTimer = 0;
  }

  if (gControls.WasPressed(JOYUP | JOYDOWN)) {
    mTimer = REPEAT_INPUT_DELAY;
  } else {
    if (mTimer > 0) {
      mTimer--;
      return;
    } else {
      mTimer = REPEAT_INPUT_INTERVAL;
    }
  }

  if (gControls.IsPressed(JOYUP) && OnNavigate(JOYUP) && mCurrentWidget->OnNavigate(JOYUP)) {
    mCurrentWidget->Deactivate();
    if (mCurrentWidget == mList.First()) {
      mCurrentWidget = (BWidget *) mList.Last();
    } else {
      mCurrentWidget = (BWidget *) mList.Prev(mCurrentWidget);
    }
    mCurrentWidget->Activate();
    // reset dKeys so next state doesn't react to any keys already pressed
    gControls.dKeys = 0;
  }

  if (gControls.IsPressed(JOYDOWN) && OnNavigate(JOYDOWN) && mCurrentWidget->OnNavigate(JOYDOWN)) {
    mCurrentWidget->Deactivate();
    if (mCurrentWidget == mList.Last()) {
      mCurrentWidget = (BWidget *) mList.First();
    } else {
      mCurrentWidget = (BWidget *) mList.Next(mCurrentWidget);
    }
    mCurrentWidget->Activate();
    // reset dKeys so next state doesn't react to any keys already pressed
    gControls.dKeys = 0;
  }
}
