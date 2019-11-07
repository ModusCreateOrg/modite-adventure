#include "Game.h"
#include "GStartLevelProcess.h"

GStartLevelProcess::GStartLevelProcess(const char *aName, TInt aLevel) : BProcess() {
  mTimer = 5 * FRAMES_PER_SECOND;
  sprintf(mText, "%s level %2d", aName, aLevel);
  TInt len = strlen(mText);
  mX = gViewPort->mRect.Width()/2 - len*12/2;
}

GStartLevelProcess::~GStartLevelProcess() {
  //
}

TBool GStartLevelProcess::RunAfter() {
  if (--mTimer < 0) {
    return EFalse;
  }

  gDisplay.renderBitmap->DrawString(gViewPort, mText, gFont16x16, mX, 32, COLOR_SHMOO_GREY, COLOR_TEXT_TRANSPARENT, -4);
  return ETrue;

}
