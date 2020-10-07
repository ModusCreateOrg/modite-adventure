//
// Created by jg on 10/6/20.
//

#include "GVictoryPlayfield.h"
#include "GAnchorSprite.h"

const TInt16 WALK_SPEED = 10 * FACTOR;

static TBool moonHitTop = EFalse;

ANIMSCRIPT walkAnimation[] = {
    ABITMAP(PLAYER_SLOT),
    ALABEL,
    ADELTA(1, 0),
    ASTEP(WALK_SPEED, IMG_WALK_RIGHT + 0),
    ADELTA(-1, 0),
    ASTEP(WALK_SPEED, IMG_WALK_RIGHT + 1),
    ADELTA(0, 0),
    ASTEP(WALK_SPEED, IMG_WALK_RIGHT + 2),
    ADELTA(0, 0),
    ASTEP(WALK_SPEED, IMG_WALK_RIGHT + 3),
    ALOOP,
};


GVictoryPlayfield::GVictoryPlayfield(GGameState *aGameState) {
  mGameState = aGameState;
  mStarfieldProcess = new GStarFieldProcess();

  mStarsColor.Set(TUint8(100), TUint8(100), TUint8(100));

//  mFont8 = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_8x8);
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);


  BBitmap *bm = BBitmap::CreateBBitmap(1, 1, SCREEN_DEPTH, MEMF_FAST);

  gResourceManager.LoadBitmap(LOGO_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBgLogo = gResourceManager.GetBitmap(BKG_SLOT);
  mBgLogo->Remap(bm);

  gResourceManager.LoadBitmap(CLOUDS_TOP_BMP, MAIN_MENU_SLOT0, IMAGE_ENTIRE);
  mBgSky = gResourceManager.GetBitmap(MAIN_MENU_SLOT0);
  mBgSkyColors = (TRGB *)malloc(sizeof(TRGB) * mBgSky->CountUsedColors());
  CacheColors(mBgSky, mBgSkyColors);
  mBgSky->Remap(bm);

  for (TInt i = 0; i < mBgSky->CountUsedColors(); i++) {
    TRGB color = mBgSky->GetColor(i);

    printf("mBgSkyColors[%i] = r(%i) b(%i) g(%i)\n", i, mBgSkyColors[i].r, mBgSkyColors[i].b, mBgSkyColors[i].g);
    printf("color[%i]        = r(%i) b(%i) g(%i)\n", i, color.r, color.b, color.g);
  }

  gResourceManager.LoadBitmap(NEAR_TREES_BMP, MAIN_MENU_SLOT1, IMAGE_ENTIRE);
  mBgNearTrees = gResourceManager.GetBitmap(MAIN_MENU_SLOT1);
  mBgNearTreesColors = (TRGB *)malloc(sizeof(TRGB) * mBgNearTrees->CountUsedColors());
  CacheColors(mBgNearTrees, mBgNearTreesColors);
  mBgNearTrees->Remap(bm);

  gResourceManager.LoadBitmap(WALKING_PATH_BMP, MAIN_MENU_SLOT2, IMAGE_ENTIRE);
  mBgWalkingPath = gResourceManager.GetBitmap(MAIN_MENU_SLOT2);
  mBgWalkingPathColors = (TRGB *)malloc(sizeof(TRGB) * mBgWalkingPath->CountUsedColors());
  CacheColors(mBgWalkingPath, mBgWalkingPathColors);
  mBgWalkingPath->Remap(bm);


  gResourceManager.LoadBitmap(MOUNTAINS_BMP, MAIN_MENU_SLOT3, IMAGE_ENTIRE);
  mBgMountains = gResourceManager.GetBitmap(MAIN_MENU_SLOT3);
  mBgMountainsColors = (TRGB *)malloc(sizeof(TRGB) * mBgMountains->CountUsedColors());
  CacheColors(mBgMountains, mBgMountainsColors);
  mBgMountains->Remap(bm);


  gResourceManager.LoadBitmap(MOON_WITH_LOGO_BMP, MAIN_MENU_SLOT4, IMAGE_ENTIRE);
  mBgMoon = gResourceManager.GetBitmap(MAIN_MENU_SLOT4);
  mMoonOffset = SCREEN_WIDTH - mBgMoon->Width() - 10;
  mBgMoon->Remap(bm);




  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);

  gResourceManager.GetBitmap(PLAYER_SLOT)->Remap(bm);
  gViewPort->mWorldX = 0;
  gViewPort->mWorldY = 0;
  gViewPort->Offset(0, 0);

  mPlayer = new GPlayerSprite(aGameState);
  mPlayer->ClearFlags(SFLAG_RENDER_DEBUG);

  mPlayer->StartAnimation(walkAnimation);
  mPlayer->x = 20;
  mPlayer->y = SCREEN_HEIGHT - 20;

  mSkyOffset = 0;
  mMountainsOffset = 0;
  mMoonOffset = moonHitTop ? 10 : 65;
  mNearTreesOffset = 0;
  mPathOffset = 0;


  // states
  // -2 fade out done, move to GAME_STATE_GAME
  // -1 fade out in progress
  // 0 fade in in progress
  // 1 fade in done

  mFadeState = 0; // -1 fade out in progress;
  mState = 0;

//  TRGB *pathPalette = mBgWalkingPath->GetPalette();
//  for (TInt16 i = 0;  i < mBgWalkingPath->CountUsedColors(); i++) {
//
//    printf("mBGWalkingPath Color[%i] = r(%i) b(%i) g(%i)\n", i, pathPalette[i].r, pathPalette[i].b, pathPalette[i].g);
//
////    if (pathPalette[i].r != 255 && pathPalette[i].r != )
//
//  }





  TRGB *sourcePalette = bm->GetPalette();
  sourcePalette[200].Set(0x45, 0x53, 0x77); // Dark blue bg
  sourcePalette[COLOR_TEXT].Set(255, 255, 255);
  sourcePalette[COLOR_TEXT_SHADOW].Set(60, 60, 60);
  sourcePalette[COLOR_TEXT_BG].Set(255, 92, 93);

  // Code for fade out.
//  for (TUint16 index = 0; index < 256; index++) {
//    TRGB c = sourcePalette[index];
//    mSavedPalette[index] = c;
//    gDisplay.SetColor(index, 0, 0, 0);
//  }

  gDisplay.SetPalette(sourcePalette, 0, 256);

}

GVictoryPlayfield::~GVictoryPlayfield() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT0);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT1);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT2);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT3);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT4);

  delete mPlayer;
  mPlayer = ENull;

  mBgLogoColors = ENull;
  mBgSkyColors = ENull;
  mBgNearTreesColors = ENull;
  mBgMountainsColors = ENull;
//  delete mBgMoonColors;
//  mBgMoonColors = ENull;
//

//  delete mFont8;
  delete mFont16;
}
//
//TInt GVictoryPlayfield::CenterText8(const char *s, TInt aY, TInt aColor, TInt aBackground) {
//  TInt x = TInt((SCREEN_WIDTH - (strlen(s) * 8)) / 2);
//  gDisplay.renderBitmap->DrawString(ENull, s, mFont8, x, aY, aColor, aBackground);
//  return 8;
//}

TInt GVictoryPlayfield::CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground) {
  TInt width = aBackground == -1 ? 12 : 16;
  TInt x     = TInt((SCREEN_WIDTH - (strlen(s) * width)) / 2);
  if (aBackground != -1) {
    gDisplay.renderBitmap->DrawString(ENull, s, mFont16, x, aY, aColor, TInt16(aBackground));
  }
  else {
    gDisplay.renderBitmap->DrawString(ENull, s, mFont16, x, aY, aColor, TInt16(aBackground), -4);
  }
  return 16;
}


void GVictoryPlayfield::Animate() {
  mPlayer->Animate();

  const TFloat pathSpeed = .75,
               nearTreesSpeed = pathSpeed - .70;

  mSkyOffset += .009;
  if ((TInt) mSkyOffset >= mBgSky->Width()) {
    mSkyOffset = 0;
  }

  if ((TInt) mMoonOffset > 10) {
    mMoonOffset -= .05;
  }
  else {
    moonHitTop = ETrue;
  }

  mNearTreesOffset += nearTreesSpeed;
  if ((TInt) mNearTreesOffset >= mBgNearTrees->Width()) {
    mNearTreesOffset = 0;
  }

  mPathOffset += pathSpeed;
  if ((TInt)mPathOffset >= mBgWalkingPath->Width()) {
    mPathOffset = 0;
  }

  mMountainsOffset += .001;
  if ((TInt)mMountainsOffset >= mBgMountains->Width()) {
    mMountainsOffset = 0;
  }


// Left the code below in so I could  noodle on it more later on.
//  // Fade In
//  if (mState < 21 && mFadeState == 0) {
//    TFloat pct = (((TFloat)mState * .05));
//
//    for (TInt color = 0; color < 256; color++) {
//      TRGB c = mSavedPalette[color];
//
//      TUint16 red = (TFloat) c.r * pct;
//      c.r = (TUint8) ((red > 0xFF) ? 0xFF : red);
//
//      TUint16 green = (TFloat) c.g * pct;
//      c.g = (TUint8) ((green > 0xFF) ? 0xFF : green);
//
//      TUint16 blue = (TFloat) c.b * pct;
//      c.b = (TUint8) ((blue > 0xFF) ? 0xFF : blue);
//
//      gDisplay.SetColor(color, c);
//    }
//  }
//  else if (mState == 21 && mFadeState == 0){
//    mFadeState = 1; // Done fading
//  }
//
//
//  // Set flag to fade out
//  if (mState == 0 && mFadeState == 1 && gGame->GetState() == GAME_STATE_GAME_PRE_ROLL) {
//    mFadeState = -1;
//  }
//
//  // Fade out
//  if (mFadeState == -1 && mState < 21) {
//    TFloat pct = ((20 - (TFloat)mState) * .05);
//
//    for (TInt color = 0; color < 256; color++) {
//      TRGB c = mSavedPalette[color];
//
//      TUint16 red = (TFloat) c.r * pct;
//      c.r = (TUint8) ((red > 0xFF) ? 0xFF : red);
//
//      TUint16 green = (TFloat) c.g * pct;
//      c.g = (TUint8) ((green > 0xFF) ? 0xFF : green);
//
//      TUint16 blue = (TFloat) c.b * pct;
//      c.b = (TUint8) ((blue > 0xFF) ? 0xFF : blue);
//
//      gDisplay.SetColor(color, c);
//    }
//  }
//  else if (mFadeState == -1 && mState >= 21) {
//    mFadeState = -2;
//    gGame->SetState(GAME_STATE_GAME);
//  }



}


void GVictoryPlayfield::Render() {
  // Paint screen in dark blue
  //200 = blue;

  gDisplay.renderBitmap->Clear(COLOR_TEXT_BG);
  gDisplay.renderBitmap->SetColor(COLOR_EXPERIENCE, mStarsColor);
//  gDisplay.renderBitmap->FillRect(ENull, 0, 0, 128, 100, COLOR_SHMOO);
  mStarfieldProcess->Render();

//  TRect rect = TRect(0, 0, mBgMoon->Width(), mBgMoon->Height());
//  const TInt moonX = SCREEN_WIDTH - mBgMoon->Width() - 20;
//  gDisplay.renderBitmap->DrawBitmap(ENull, mBgMoon, rect, moonX, (TInt)mMoonOffset);

  const TInt walkingPathOffset = DISPLAY_HEIGHT - mBgWalkingPath->Height();

  DrawScrolledBackground(mBgSky, mSkyOffset, 0);
  DrawScrolledBackground(mBgMountains, mMountainsOffset, 70);
  DrawScrolledBackground(mBgNearTrees, mNearTreesOffset, 80);
  DrawScrolledBackground(mBgWalkingPath, mPathOffset, walkingPathOffset);

    // Draw the Modite Adventure Logo
//  gDisplay.renderBitmap->DrawBitmapTransparent(
//      ENull,  // ViewPort
//      mBgLogo, // bitmap
//      TRect(0, 0, mBgLogo->Width() - 1, mBgLogo->Height() - 1),  // src rect
//      50,   // Dest X
//      10    // Dest Y
//  );

  mPlayer->Render(gViewPort);

  mState++;
}



void GVictoryPlayfield::DrawScrolledBackground(BBitmap *aBitmap, TFloat aOffsetX, TUint aVerticalOffset) {

  TInt16 intOffsetX = (TInt16)aOffsetX,
      canvasWidth = gDisplay.renderBitmap->Width(),
      remainDrawWidth = canvasWidth, // Remaining width to draw, since we'll have to do multiple passes
      bgWidth = aBitmap->Width(),
      priorDrawWidth = 0,
      imgWidthDelta = bgWidth - intOffsetX;

  // Background is too big for the canvas, so just draw the full canvas width and be done!
  if (imgWidthDelta >= canvasWidth) {
    TRect rect = TRect(intOffsetX, 0, bgWidth, aBitmap->Height());

    gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect, 0, aVerticalOffset);
    return;
  }


  while (remainDrawWidth > 0) {
    TInt16 drawWidth = 0;

    // Background is too big for the canvas
    if (bgWidth > canvasWidth) {
      if (remainDrawWidth == canvasWidth) {
        drawWidth = imgWidthDelta;
        TRect rect = TRect(intOffsetX, 0, bgWidth, aBitmap->Height());

        gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect, 0, aVerticalOffset);
      }
      else {
        drawWidth = SCREEN_WIDTH - priorDrawWidth;
        TRect rect = TRect(0, 0, drawWidth, aBitmap->Height());

        gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect, priorDrawWidth, aVerticalOffset);
      }
    }
    else { // Backgrounds that are too small for the canvas
      if (remainDrawWidth == canvasWidth) {
        drawWidth = bgWidth - intOffsetX;
        TRect rect = TRect(intOffsetX, 0, bgWidth, aBitmap->Height());

        gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect, 0, aVerticalOffset);
      }
      else {
        drawWidth = (remainDrawWidth > bgWidth) ? bgWidth : remainDrawWidth;
        TRect rect = TRect(0, 0, drawWidth, aBitmap->Height());

        gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect,  priorDrawWidth, aVerticalOffset);
      }
    }


    if (drawWidth == 0) {
      return;
    }

    priorDrawWidth += drawWidth;
    remainDrawWidth -= drawWidth;

    if (remainDrawWidth < 1) {
      return;
    }
  }
}
