#ifndef MODITE_GVICTORYCREDITSPROCESS_H
#define MODITE_GVICTORYCREDITSPROCESS_H


class GVictoryCreditsProcess {

public:
  TBool mAnimateCharacterIn;
  TUint8 mLogoColorIndices[4]{};
  TRGB mLogoColors[4]{};

  void SetLabsForegroundIndex(TUint8 aForegroundIndex) {
    mModusLogoForegroundIdx = aForegroundIndex;
  }
  void SetLabsBackgroundIndex(TUint8 aBackgroundIndex) {
    mModusLogoBackgroundIdx = aBackgroundIndex;
  }

  TInt16 GetText() {
    return mText;
  }

  GVictoryCreditsProcess(TUint8 aTextColorIndex) {
    mTextColorIndex = aTextColorIndex;
    mText  = TEXT_PREROLL;
    mState = STATE_PRE_FADEIN;
    mColorFloor = 30;
    mColorDivider = 80; // Higher is slower
    mColor = mColorFloor;
    mTimer = 30;
    mAnimateCharacterIn = EFalse;

    mStateTimer = 0;

    mFont8 = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_8x8);
    mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
  }

  ~GVictoryCreditsProcess() {
//    gResourceManager.ReleaseBitmapSlot(ENVIRONMENT_SLOT);
//    delete mBgLabsLogo;
//    mBgLabsLogo = ENull;

    delete mFont8;
    delete mFont16;
  }

  TBool Run() {
    RenderText();
    switch (mState) {
      case STATE_PRE_FADEIN:
//        printf("PreFadeInState\n");
        return PreFadeInState();
      case STATE_FADEOUT:
//        printf("FadeOutState\n");
        return FadeOutState();
      case STATE_FADEIN:
//        printf("FadeInState\n");
        return FadeInState();
      case STATE_POST_FADEIN:
//        printf("PostFadeInState\n");
        return PostFadeInState();
      default:
        Panic("TextProcess invalid state");
    }
    return ETrue;
  }

  TInt16 RenderGameLogo() {
    BBitmap *bm = gResourceManager.GetBitmap(BKG_SLOT);

    const TRect rect = TRect(0, 0, bm->Width(), bm->Height());
    const TInt16 x = (SCREEN_WIDTH / 2) - (bm->Width() / 2);
    const TInt16 y = (SCREEN_HEIGHT / 2) - (bm->Height() / 2) - 40;

    for (TInt8 i = 0; i < 5; i++) {
      TFloat pct = ((TFloat)mColor / 255);

      gDisplay.renderBitmap->SetColor(
        mLogoColorIndices[i],
        (TFloat)mLogoColors[i].r * pct,
        (TFloat)mLogoColors[i].g * pct,
        (TFloat)mLogoColors[i].b * pct
      );
    }

    gDisplay.renderBitmap->DrawBitmapTransparent(ENull, bm, rect, x, y);
    return y + bm->Height() + 10;
  }

  void RenderModusLogo() {
    BBitmap *labsLogo = gResourceManager.GetBitmap(ENVIRONMENT_SLOT);
    const TRect rect = TRect(0, 0, labsLogo->Width(), labsLogo->Height());
    const TInt16 labsLogoX = (SCREEN_WIDTH / 2) - (labsLogo->Width() / 2);
    const TInt16 labsLogoY = (SCREEN_HEIGHT / 2) - (labsLogo->Height() / 2) - 20;
    gDisplay.renderBitmap->DrawBitmapTransparent(ENull, labsLogo, rect, labsLogoX, labsLogoY);
  }

  void RenderText() {
    gDisplay.SetColor(mTextColorIndex, mColor, mColor, mColor);
    gDisplay.SetColor(COLOR_TEXT_SHADOW, 60, 60, 60);

    TInt y = 0;
    TUint8  numLines = 0;
    if (mState != STATE_PRE_FADEIN) {
      switch (mText) {
        case TEXT_EMPTY:
        case TEXT_PREROLL:
          ;;
          break;
        case TEXT_GAME_TITLE:
          y = RenderGameLogo();
          CenterText16("Brought to you by", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_MODUS_LABS:
          mAnimateCharacterIn = ETrue;
          RenderModusLogo();
          break;
        case TEXT_GAME_DESIGN:
          numLines = 4;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("GAME DESIGN", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Jay Garcia", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Michael Tintiuc", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Aaron Goldman", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Dmytro Kovalenko", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_VISUAL_DESIGN:
          numLines = 4;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("VISUAL DESIGN", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Aaron Goldman", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Dmytro Kovalenko", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Jay Garcia", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_PROGRAMMING:
          numLines = 4;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("PROGRAMMING", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Mike Schwartz", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Michael Tintiuc", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Aaron Goldman", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Jay Garcia", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_CI:
          numLines = 4;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("AUTOMATION", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Don Anderson", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Michael Tintiuc", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Thomas Collins", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Richard Bullington-McGuire", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_PM:
          numLines = 5;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("LEVEL DESIGN", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Aaron Goldman", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Lucas Garibaldi", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Santiago Kent", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Jay Garcia", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_SOUND:
          numLines = 3;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("MUSIC AND SFX", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Jay Garcia", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Victor Guzman", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_QA:
          numLines = 7;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("QUALITY ASSURANCE", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Raul Dan Mocian", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Fanny Cespedes", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Thomas Collins", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Miguel Montano", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Pedro Hyvo", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Samantha Park", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Josh Kochelek", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_INSTRUCTIONS:
          numLines = 2;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("GAME INSTRUCTIONS", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Dmytro Kovalenko", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Shiv Sharma", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_LOGISTICS:
          numLines = 3;
          y = (SCREEN_HEIGHT / 2) - ((24 + (numLines * 16)) / 2);
          y += CenterText16("LOGISTICS", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Ashley Benton", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Dmytro Kovalenko", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Gabrielle Viot", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_DIRECTOR:
          y = (SCREEN_HEIGHT / 2) - ((24 + (1 * 16)) / 2);
          y += CenterText16("DIRECTOR", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText8("Jay Garcia", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_GREETS:
          y = (SCREEN_HEIGHT / 2) - ((24 + (2 * 16)) / 2);
          y += CenterText16("Greets to the LDK", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          y += CenterText16("Game dev community!", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT);
          break;
        case TEXT_THANK_YOU:
          y = (SCREEN_HEIGHT / 2) - ((24 + (1 * 16)) / 2);
          y += CenterText16("Thanks for playing!", y, mTextColorIndex, COLOR_TEXT_TRANSPARENT) + 8;
          break;
      }
    }
  }


protected:
  TInt CenterText8(const char *s, TInt aY, TInt aColor, TInt aBackground) {
    TInt x = TInt((SCREEN_WIDTH - (strlen(s) * 8)) / 2);
    gDisplay.renderBitmap->DrawString(ENull, s, gFont8x8, x + 1, aY + 1, COLOR_SHADOW, aBackground);
    gDisplay.renderBitmap->DrawString(ENull, s, gFont8x8, x, aY, aColor, aBackground);
    return 8 + 2;
  }

  TInt CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground) {
    TInt width = aBackground == -1 ? 12 : 16;
    TInt x = TInt((SCREEN_WIDTH - (strlen(s) * width)) / 2);
    if (aBackground != -1) {
      gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x + 1, aY + 1, COLOR_SHADOW, TInt16(aBackground));
      gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x, aY, aColor, TInt16(aBackground));
    }
    else {
      gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x + 1, aY + 1, COLOR_SHADOW, TInt16(aBackground), -4);
      gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x, aY, aColor, TInt16(aBackground), -4);
    }
    return 16 + 4;
  }

  TInt mColorDivider;
  TInt mColorFloor;

  TBool FadeInState() {
    mColor += (255 / mColorDivider);
    if (mColor > 255) {
      mColor = 255;
    }

    gDisplay.SetColor(mModusLogoForegroundIdx, mColor, mColor, mColor);

    auto shadowColor = (TUint8)(mColorDivider * 0x40);
    gDisplay.SetColor(mModusLogoBackgroundIdx, shadowColor, shadowColor, shadowColor);

    if (mColor == 255) {
      if (mText != TEXT_THANK_YOU) {
        mState = STATE_POST_FADEIN;
      }
      mTimer = 11 * 30;
    }
    return ETrue;
  }

  TBool FadeOutState() {
    mColor -= (255 / mColorDivider);
    if (mColor < mColorFloor) {
      mColor = mColorFloor;
    }

    gDisplay.SetColor(mModusLogoForegroundIdx, mColor, mColor, mColor);

    auto shadowColor = (TUint8)(mColorDivider * 0x40);
    gDisplay.SetColor(mModusLogoBackgroundIdx, shadowColor, shadowColor, shadowColor);

    gDisplay.SetColor(COLOR_TEXT, mColor, mColor, mColor);

    if (mColor == mColorFloor) {
      mState = STATE_PRE_FADEIN;
      mTimer = 30;
    }

    return ETrue;
  }

  TBool PostFadeInState() {
    mTimer--;
    if (mTimer < 0) {
      mState = STATE_FADEOUT;
    }

    return ETrue;
  }

  TBool PreFadeInState() {
    mTimer--;
//    printf("mTimer %i\n", mTimer);
    if (mTimer < 0) {
      mState = STATE_FADEIN;

      switch (mText) {
        case TEXT_EMPTY:
          break;
        case TEXT_PREROLL:
          mText = TEXT_GAME_TITLE;
          break;
        case TEXT_GAME_TITLE:
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
          mText = TEXT_QA;
          break;
        case TEXT_QA:
          mText = TEXT_INSTRUCTIONS;
          break;
        case TEXT_INSTRUCTIONS:
          mText = TEXT_LOGISTICS;
          break;
        case TEXT_LOGISTICS:
          mText = TEXT_DIRECTOR;
        case TEXT_DIRECTOR:
          mText = TEXT_GREETS;
          break;
        case TEXT_GREETS:
          mText = TEXT_THANK_YOU;
          break;
        case TEXT_THANK_YOU:
          mText = TEXT_THANK_YOU;
          break;
      }
    }

//    printf("New mText = %i\n", mText);

    return ETrue;
  }

  TUint8  mTextColorIndex;
  TUint8  mModusLogoForegroundIdx;
  TUint8  mModusLogoBackgroundIdx;
  TInt mTimer, mColor;
  BFont *mFont8;
  BFont *mFont16;

  TUint32 mStateTimer;

  enum {
    STATE_FADEIN,
    STATE_FADEOUT,
    STATE_POST_FADEIN,
    STATE_PRE_FADEIN,
  } mState;

  enum {
    TEXT_EMPTY,
    TEXT_PREROLL,
    TEXT_GAME_TITLE,
    TEXT_MODUS_LABS,
    TEXT_GAME_DESIGN,
    TEXT_VISUAL_DESIGN,
    TEXT_PROGRAMMING,
    TEXT_CI,
    TEXT_PM,
    TEXT_SOUND,
    TEXT_QA,
    TEXT_INSTRUCTIONS,
    TEXT_LOGISTICS,
    TEXT_DIRECTOR,
    TEXT_GREETS,
    TEXT_THANK_YOU,
  } mText;

};


#endif //MODITE_GVICTORYCREDITSPROCESS_H
