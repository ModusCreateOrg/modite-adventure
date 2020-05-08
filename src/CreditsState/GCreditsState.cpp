#include "Game.h"
#include "GCreditsState.h"

class CreditsProcess : public BProcess {
public:
  CreditsProcess() {
    mFont  = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
    mText  = "CREDITS";
    mState = STATE_FADEIN;
    mColor = 0;
  }

  ~CreditsProcess() {
    delete mFont;
    mFont = ENull;
  }

protected:
  enum {
    STATE_FADEIN,
    STATE_FADEOUT,
    STATE_WAIT,
  } mState;

  const char *mText;
  BFont *mFont;

  TInt RenderString(const char *aString, TInt aY) {
    TInt width = TInt(strlen(aString) * 12);
    TInt x     = (SCREEN_WIDTH - width) / 2;
    gDisplay.renderBitmap->DrawString(ENull, aString, mFont, x, aY, COLOR_TEXT, COLOR_TEXT_TRANSPARENT, -4);
    return 18;
  }

  void RenderText() {
    // TODO: Render text based on state, see GENUS for an example
    RenderString(mText, 144 - 24);
  }

  TInt mTimer, mColor;

  TBool FadeInState() {
    mColor += (255 / 15);
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

  TBool FadeOutState() {
    mColor -= (255 / 15);
    if (mColor < 0) {
      mColor = 0;
    }
    gDisplay.SetColor(COLOR_TEXT, mColor, mColor, mColor);
    if (mColor == 0) {
      mState = STATE_FADEIN;
    }
    return ETrue;
  }

  TBool WaitState() {
    if (--mTimer < 0) {
      mState = STATE_FADEOUT;
    }
    return ETrue;
  }

  TBool RunBefore() {
    if (gControls.WasPressed(BUTTON_ANY)) {
      gGame->SetState(GAME_STATE_MAIN_MENU);

// TODO: @jaygarcia
#ifdef ENABLE_AUDIO
      gSoundPlayer.SfxMenuOut();
#endif
      return EFalse;
    }
    return ETrue;
  }

  TBool RunAfter() {
    RenderText();
    switch (mState) {
      case STATE_FADEIN:
        return FadeInState();
      case STATE_FADEOUT:
        return FadeOutState();
      case STATE_WAIT:
        return WaitState();
      default:
        Panic("TextProcess invalid state");
    }
    return ETrue;
  }
};

class GCreditsPlayfield : public BPlayfield {
public:
  GCreditsPlayfield() {
    gResourceManager.LoadBitmap(TITLE_BMP, BKG_SLOT, IMAGE_ENTIRE);
    mBackground = gResourceManager.GetBitmap(BKG_SLOT);
    gDisplay.SetPalette(mBackground);
    gDisplay.SetColor(0, 0, 0, 0);
  }

  virtual ~GCreditsPlayfield() {
    gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  }

public:
  void Render() {
    gDisplay.renderBitmap->CopyPixels(mBackground);
  }

public:
  BBitmap *mBackground;
};

GCreditsState::GCreditsState() : BGameEngine(gViewPort) {
  mPlayfield = new GCreditsPlayfield();
  AddProcess(new CreditsProcess());
}

GCreditsState::~GCreditsState() {}

