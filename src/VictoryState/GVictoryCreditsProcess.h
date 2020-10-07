#ifndef MODITE_GVICTORYCREDITSPROCESS_H
#define MODITE_GVICTORYCREDITSPROCESS_H


class GVictoryCreditsProcess {

public:
  GVictoryCreditsProcess(TUint8 aColorIndex) {
    mColorIndex = aColorIndex;
    mText  = TEXT_PRE_ROLL;
    mState = STATE_FADEIN;
    mColorFloor = 60;
    mColorDivider = 90;
    mColor = mColorFloor;

    mStateTimer = 0;

    mFont8 = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_8x8);
    mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
  }

  ~GVictoryCreditsProcess() {
    delete mFont8;
    delete mFont16;
  }

  TBool Run() {
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

  void RenderText() {
    gDisplay.SetColor(mColorIndex, mColor, mColor, mColor);

    TInt y;

    switch (mText) {
      case TEXT_PRE_ROLL:

        break;
      case TEXT_MODUS_LABS:
        //TODO: Turn to image
        y = (SCREEN_HEIGHT / 2) - ((24 + (1 * 16))  / 2);
        y += CenterText8("MADE BY", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        y += CenterText8("Modus Labs", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        break;
      case TEXT_GAME_DESIGN:
        y = (SCREEN_HEIGHT / 2) - ((24 + (4 * 16))  / 2);
        y += CenterText8("GAME DESIGN", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        y += CenterText8("Jay Garcia", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Michael Tintiuc", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Aaron Goldman", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Dmytro Kovalenko", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        break;
      case TEXT_VISUAL_DESIGN:
        y = (SCREEN_HEIGHT / 2) - ((24 + (3 * 16))  / 2);
        y += CenterText8("VISUAL DESIGN", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        y += CenterText8("Aaron Goldman", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Dmytro Kovalenko", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Jay Garcia", y,mColorIndex,  COLOR_TEXT_TRANSPARENT);
        break;
      case TEXT_PROGRAMMING:
        y = (SCREEN_HEIGHT / 2) - ((24 + (3 * 16))  / 2);
        y += CenterText8("PROGRAMMING", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        y += CenterText8("Mike Schwartz", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Michael Tintiuc", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Aaron Goldman", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        break;
      case TEXT_CI:
        y = (SCREEN_HEIGHT / 2) - ((24 + (3 * 16))  / 2);
        y += CenterText8("CONTINUOUS INTEGRATION", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        y += CenterText8("Don Anderson", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Richard Bullington-McGuire", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Michael Tintiuc", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        break;
      case TEXT_PM:
        y = (SCREEN_HEIGHT / 2) - ((24 + (3 * 16))  / 2);
        y += CenterText8("LEVEL DESIGN", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        y += CenterText8("Aaron Goldman", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Lucas Garibaldi", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Jay Garcia", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        break;
      case TEXT_SOUND:
        y = (SCREEN_HEIGHT / 2) - ((24 + (2 * 16))  / 2);
        y += CenterText8("MUSIC AND SFX", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        y += CenterText8("Jay Garcia", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        y += CenterText8("Victor Guzman", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        break;
      case TEXT_DIRECTOR:
        y = (SCREEN_HEIGHT / 2) - ((24 + (1 * 16))  / 2);
        y += CenterText8("DIRECTOR", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        y += CenterText8("Jay Garcia", y, mColorIndex, COLOR_TEXT_TRANSPARENT);
        break;
      case TEXT_THANK_YOU:
        y = (SCREEN_HEIGHT / 2) - ((24 + (1 * 16))  / 2);
        y += CenterText8("Thanks for playing!", y, mColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
        break;
    }
  }


protected:
  TInt CenterText8(const char *s, TInt aY, TInt aColor, TInt aBackground) {
    TInt x = TInt((SCREEN_WIDTH - (strlen(s) * 8)) / 2);
    gDisplay.renderBitmap->DrawString(ENull, s, gFont8x8, x, aY, aColor, aBackground);
    return 8;
  }

  TInt CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground) {
    TInt width = aBackground == -1 ? 12 : 16;
    TInt x = TInt((SCREEN_WIDTH - (strlen(s) * width)) / 2);
    if (aBackground != -1) {
      gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x, aY, aColor, TInt16(aBackground));
    }
    else {
      gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x, aY, aColor, TInt16(aBackground), -4);
    }
    return 16;
  }

  TInt mColorDivider;
  TInt mColorFloor;

  TBool FadeInState() {
    mColor += (255 / mColorDivider);
    if (mColor > 255) {
      mColor = 255;
    }

    gDisplay.SetColor(mColorIndex, mColor, mColor, mColor);

    if (mColor == 255) {
      mState = STATE_WAIT;
      mTimer = 10 * 30;
    }
    return ETrue;
  }

  TBool FadeOutState() {
    mColor -= (255 / mColorDivider);
    if (mColor < mColorFloor) {
      mColor = mColorFloor;
    }

    gDisplay.SetColor(COLOR_TEXT, mColor, mColor, mColor);

    if (mColor == mColorFloor) {
      mState = STATE_FADEIN;
      switch (mText) {
        case TEXT_PRE_ROLL:
          mText = TEXT_MODUS_LABS;
          break;
        case TEXT_MODUS_LABS:
          mText = TEXT_GAME_DESIGN;
          break;
        case TEXT_GAME_DESIGN:
          mText = TEXT_VISUAL_DESIGN;
          break;
        case TEXT_VISUAL_DESIGN:
          mText = TEXT_PROGRAMMING;
          break;
        case TEXT_PROGRAMMING:
          mText = TEXT_CI;
          break;
        case TEXT_CI:
          mText = TEXT_PM;
          break;
        case TEXT_PM:
          mText = TEXT_SOUND;
          break;
        case TEXT_SOUND:
          mText = TEXT_DIRECTOR;
          break;
        case TEXT_DIRECTOR:
          mText = TEXT_THANK_YOU;
          break;
      }
    }

    return ETrue;
  }

  TBool WaitState() {
    mTimer--;
    if (mTimer < 0) {
      mState = STATE_FADEOUT;
    }
    return ETrue;
  }

  TUint8  mColorIndex;
  TInt mTimer, mColor;

  BFont *mFont8;
  BFont *mFont16;

  TUint32 mStateTimer;

  enum {
    STATE_FADEIN,
    STATE_FADEOUT,
    STATE_WAIT,
  } mState;

  enum {
    TEXT_PRE_ROLL,
    TEXT_MODUS_LABS,
    TEXT_GAME_DESIGN,
    TEXT_VISUAL_DESIGN,
    TEXT_PROGRAMMING,
    TEXT_CI,
    TEXT_PM,
    TEXT_SOUND,
    TEXT_DIRECTOR,
    TEXT_THANK_YOU,
  } mText;

};


#endif //MODITE_GVICTORYCREDITSPROCESS_H
