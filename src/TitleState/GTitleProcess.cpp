#include "GTitleProcess.h"

static const char *title_message1 = "Press a button";
static const char *title_message2 = "Press a button";

GTitleProcess::GTitleProcess() {
  mColor       = 0;
  mState       = STATE_FADEIN;
  mFont        = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
  mCurrentText = title_message1;
#ifdef ENABLE_AUDIO
  gSoundPlayer.PlayMusic(INTRO_XM);
#endif
}

GTitleProcess::~GTitleProcess() {
  delete mFont;
}

TBool GTitleProcess::RunBefore() {
  RenderText();

  return ETrue;
}

TBool GTitleProcess::FadeInState() {
  mColor += (255 / 30);
  if (mColor > 255) {
    mColor = 255;
  }
  gDisplay.SetColor(COLOR_TEXT, mColor, mColor, mColor);
  if (mColor == 255) {
    mState = STATE_WAIT;
    mTimer = 2 * 30;
  }
  return ETrue;
}

TBool GTitleProcess::FadeOutState() {
  mColor -= (255 / 30);
  if (mColor < 0) {
    mColor = 0;
  }
  gDisplay.SetColor(COLOR_TEXT, mColor, mColor, mColor);
  if (mColor == 0) {
    mState       = STATE_FADEIN;
    mCurrentText = mCurrentText == title_message1 ? title_message2 : title_message1;
  }
  return ETrue;
}

TBool GTitleProcess::WaitState() {
  mTimer--;
  if (mTimer < 0) {
    mState = STATE_FADEOUT;
  }
  return ETrue;
}

void GTitleProcess::RenderText() {
  if (!mCurrentText) {
    return;
  }
  TInt width = TInt(strlen(mCurrentText) * 12);
  TInt x     = ((320 - width) / 2) - 5;
  gDisplay.renderBitmap->DrawString(ENull, mCurrentText, mFont, x, 160, COLOR_TEXT, -1, -4);
}

TBool GTitleProcess::RunAfter() {
  if (gControls.WasPressed(BUTTON_ANY)) { // } || --mTimer <= 0) {
    gGame->SetState(GAME_STATE_GAME);
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxNewBall();
#endif
    return EFalse;
  }
  switch (mState) {
    case STATE_FADEIN:
      return FadeInState();
    case STATE_FADEOUT:
      return FadeOutState();
    case STATE_WAIT:
      return WaitState();
    default:
      Panic("GTitleProcess invalid state");
  }
  return ETrue;
}

