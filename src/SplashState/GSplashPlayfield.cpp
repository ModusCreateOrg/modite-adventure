#include "GSplashPlayfield.h"


GSplashPlayfield::GSplashPlayfield() {
  gResourceManager.LoadBitmap(SPLASH_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBackground = gResourceManager.GetBitmap(BKG_SLOT);
  gDisplay.SetPalette(mBackground);
  mFrame = 0;
  mYStep = 3;
  mAnimatedColorValue = -255;

  TUint16 width  = mBackground->Width(),
          height = mBitmapHeight = mBackground->Height();

  mBackgroundX = (SCREEN_WIDTH * .5) - (width * .5);
  mBackgroundY = (SCREEN_HEIGHT * .5) - (height * .5);

  // Cache colors
  TRGB *source = mBackground->GetPalette();
  for (TInt color = 0; color < COLOR_SHADOW; color++) {
    TRGB c = source[color];
    mSavedPalette[color] = c;
    gDisplay.SetColor(color, 200,200,200);
  }

  gDisplay.SetColor(252, 150,150,150);
  gDisplay.SetColor(253, 200,200,200);
  gDisplay.SetColor(254, 0,0,0);
  gDisplay.SetColor(COLOR_TEXT_BG, 0,0,0);

#ifdef ENABLE_AUDIO
  gSoundPlayer.PlayMusic(MAIN_MENU_XM);
#endif
}

GSplashPlayfield::~GSplashPlayfield() {
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
}

void GSplashPlayfield::Render() {
  TRect rect;

  rect.Width(mBackground->Width());

  TUint16 height = 0;

  if (mYPosition > mBackgroundY) {
    height = mYPosition - mBackgroundY;
  }

  if (height > mBitmapHeight) {
    height = mBitmapHeight;
  }
  rect.Height(height);

  gDisplay.renderBitmap->Clear(COLOR_TEXT_BG);

  gDisplay.renderBitmap->DrawBitmapTransparent(ENull, mBackground, rect, mBackgroundX, mBackgroundY);

  if (mYPosition < SCREEN_HEIGHT) {

    gDisplay.renderBitmap->DrawFastHLine(ENull, 0, mYPosition - 1, 319, (TUint8)252);
    gDisplay.renderBitmap->DrawFastHLine(ENull, 0, mYPosition, 319, (TUint8) 253);
  }
  mFrame++;
}

void GSplashPlayfield::Animate() {
  if (mYPosition < SCREEN_HEIGHT) {
    mYPosition += mYStep;
  }
  else {
    if (mAnimatedColorValue == -255) {
      mAnimatedColorValue = 0xFF;
    }
  }
  // -1 means stop!

  if (mAnimatedColorValue > 0) {
    mAnimatedColorValue -= mYStep;

    if (mAnimatedColorValue < 0) {
      mAnimatedColorValue = 0;
    }

    for (TInt color = 0; color < 26; color++) {
      TRGB c = mSavedPalette[color];

      TRGB newC;

      TUint16 red =  c.r + (TUint8)mAnimatedColorValue;
      newC.r =   (TUint8)((red > 0xFF) ? 0xFF :  red);

      TUint16 green =  c.g + (TUint8)mAnimatedColorValue;
      newC.g =  (TUint8)((green > 0xFF) ? 0xFF : green);

      TUint16 blue =  c.b + (TUint8)mAnimatedColorValue;
      newC.b =  (TUint8)((blue > 0xFF) ? 0xFF : blue);

      gDisplay.SetColor(color, newC);
    }
  }





}
