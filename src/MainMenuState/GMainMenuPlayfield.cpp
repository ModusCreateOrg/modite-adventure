#include "GMainMenuPlayfield.h"
#include "GAnchorSprite.h"

const TInt16 WALKSPEED = 4 * FACTOR;

static TBool moonHitTop = EFalse;

ANIMSCRIPT walkRightAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ALABEL,
  ADELTA(1, 0),
  ASTEP(WALKSPEED, IMG_WALK_RIGHT + 0),
  ADELTA(-1, 0),
  ASTEP(WALKSPEED, IMG_WALK_RIGHT + 1),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_RIGHT + 2),
  ADELTA(0, 0),
  ASTEP(WALKSPEED, IMG_WALK_RIGHT + 3),
  ALOOP,
};


GMainMenuPlayfield::GMainMenuPlayfield(GGameState *aGameState) {
  mGameState = aGameState;
//  mFont8 = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_8x8);
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);


  BBitmap *bm = BBitmap::CreateBBitmap(1, 1, SCREEN_DEPTH, MEMF_FAST);

  gResourceManager.LoadBitmap(LOGO_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBgLogo = gResourceManager.GetBitmap(BKG_SLOT);
  mBgLogo->Remap(bm);

  gResourceManager.LoadBitmap(SKY_BMP, MAIN_MENU_SLOT1, IMAGE_ENTIRE);
  mBgSky = gResourceManager.GetBitmap(MAIN_MENU_SLOT1);
  mBgSky->Remap(bm);


  gResourceManager.LoadBitmap(NEAR_TREES_BMP, MAIN_MENU_SLOT2, IMAGE_ENTIRE);
  mBgNearTrees = gResourceManager.GetBitmap(MAIN_MENU_SLOT2);
  mBgNearTrees->Remap(bm);

  gResourceManager.LoadBitmap(WALKING_PATH_BMP, MAIN_MENU_SLOT3, IMAGE_ENTIRE);
  mBgWalkingPath = gResourceManager.GetBitmap(MAIN_MENU_SLOT3);
  mBgWalkingPath->Remap(bm);

  gResourceManager.LoadBitmap(MOUNTAINS_BMP, MAIN_MENU_SLOT4, IMAGE_ENTIRE);
  mBgMountains = gResourceManager.GetBitmap(MAIN_MENU_SLOT4);
  mBgMountains->Remap(bm);

  gResourceManager.LoadBitmap(MOON_WITH_LOGO_BMP, MAIN_MENU_SLOT5, IMAGE_ENTIRE);
  mBgMoon = gResourceManager.GetBitmap(MAIN_MENU_SLOT5);
  mBgMoon->Remap(bm);
  mMoonOffset = SCREEN_WIDTH - mBgMoon->Width() - 10;

  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  TBool res = gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);

  printf("Result = %i\n", res);
  gResourceManager.GetBitmap(PLAYER_SLOT)->Remap(bm);
//  BBitmap *playerBM = gResourceManager.GetBitmap(PLAYER_SLOT);
//  printf("playerBM = %p\n", playerBM);
//  printf("gViewPort = %p\n", gViewPort);
  gViewPort->mWorldX = 0;
  gViewPort->mWorldY = 0;
  gViewPort->Offset(0, 0);

  mPlayer = new GPlayerSprite(aGameState);
  mPlayer->ClearFlags(SFLAG_RENDER_DEBUG);

  mPlayer->StartAnimation(walkRightAnimation);
  mPlayer->x = 10;
  mPlayer->y = SCREEN_HEIGHT - 18;

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

GMainMenuPlayfield::~GMainMenuPlayfield() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT1);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT2);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT3);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT4);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT5);

  delete mPlayer;
  mPlayer = ENull;

//  delete mFont8;
  delete mFont16;
}
//
//TInt GMainMenuPlayfield::CenterText8(const char *s, TInt aY, TInt aColor, TInt aBackground) {
//  TInt x = TInt((SCREEN_WIDTH - (strlen(s) * 8)) / 2);
//  gDisplay.renderBitmap->DrawString(ENull, s, mFont8, x, aY, aColor, aBackground);
//  return 8;
//}

TInt GMainMenuPlayfield::CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground) {
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


void GMainMenuPlayfield::Animate() {
  mPlayer->Animate();

  mSkyOffset += .3;
  if ((TInt) mSkyOffset >= mBgSky->Width()) {
    mSkyOffset = 0;
  }

  if ((TInt) mMoonOffset > 10) {
    mMoonOffset -= .05;
  }
  else {
    moonHitTop = ETrue;
  }

  mNearTreesOffset += .02;
  if ((TInt) mNearTreesOffset >= mBgNearTrees->Width()) {
    mNearTreesOffset = 0;
  }

  mPathOffset += .75;
  if ((TInt)mPathOffset >= mBgWalkingPath->Width()) {
    mPathOffset = 0;
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


void GMainMenuPlayfield::Render() {
  // Paint screen in dark blue
  gDisplay.renderBitmap->Clear(200);

  TRect rect = TRect(0, 0, mBgMoon->Width(), mBgMoon->Height());

  const TInt moonX = SCREEN_WIDTH - mBgMoon->Width() - 20;
  gDisplay.renderBitmap->DrawBitmap(ENull, mBgMoon, rect, moonX, (TInt)mMoonOffset);

  const TInt walkingPathOffset = DISPLAY_HEIGHT - mBgWalkingPath->Height(),
             nearTreesOffset   = DISPLAY_HEIGHT - mBgNearTrees->Height();

  DrawScrolledBackground(mBgSky, mSkyOffset, 0, ETrue);
  DrawScrolledBackground(mBgMountains, mMountainsOffset, 50, ETrue);
  DrawScrolledBackground(mBgNearTrees, mNearTreesOffset, nearTreesOffset, ETrue);
  DrawScrolledBackground(mBgWalkingPath, mPathOffset, walkingPathOffset, ETrue);

  gDisplay.renderBitmap->DrawBitmapTransparent(
     ENull,  // ViewPort
     mBgLogo, // bitmap
     TRect(0, 0, mBgLogo->Width() - 1, mBgLogo->Height() - 1),  // src rect
     50,   // Dest X
     10    // Dest Y
   );

  mPlayer->Render(gViewPort);

  mState++;
}



void GMainMenuPlayfield::DrawScrolledBackground(BBitmap *aBitmap, TFloat aOffsetX, TUint aVerticalOffset, TBool aDrawTransparent) {

  TInt16 intOffsetX = (TInt16)aOffsetX,
      canvasWidth = gDisplay.renderBitmap->Width(),
      remainDrawWidth = canvasWidth, // Remaining width to draw, since we'll have to do multiple passes
      bgWidth = aBitmap->Width(),
      priorDrawWidth = 0;


  TInt16 imgWidthDelta = bgWidth - intOffsetX;

  // Background is too big for the canvas, so just draw the full canvas width and be done!
  if (imgWidthDelta >= canvasWidth) {
    TRect rect = TRect(intOffsetX, 0, bgWidth, aBitmap->Height());

    if (aDrawTransparent) {
      gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect, 0, aVerticalOffset);
    }
    else {
      gDisplay.renderBitmap->DrawBitmap(ENull, aBitmap, rect, 0, aVerticalOffset);
    }
    return;
  }


  while (remainDrawWidth > 0) {
    TInt16 drawWidth = 0;

    // Background is too big for the canvas
    if (bgWidth > canvasWidth) {
      if (remainDrawWidth == canvasWidth) {
        drawWidth = imgWidthDelta;
        TRect rect = TRect(intOffsetX, 0, bgWidth, aBitmap->Height());

        if (aDrawTransparent) {
          gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect, 0, aVerticalOffset);
        }
        else {
          gDisplay.renderBitmap->DrawBitmap(ENull, aBitmap, rect, 0, aVerticalOffset);
        }

      }
      else {
        drawWidth = SCREEN_WIDTH - priorDrawWidth;
        TRect rect = TRect(0, 0, drawWidth, aBitmap->Height());

        if (aDrawTransparent) {
          gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect, priorDrawWidth, aVerticalOffset);
        }
        else {
          gDisplay.renderBitmap->DrawBitmap(ENull, aBitmap, rect, priorDrawWidth, aVerticalOffset);
        }
      }
    }
    else { // Backgrounds that are too small for the canvas
      if (remainDrawWidth == canvasWidth) {
        drawWidth = bgWidth - intOffsetX;
        TRect rect = TRect(intOffsetX, 0, bgWidth, aBitmap->Height());

        if (aDrawTransparent) {
          gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect, 0, aVerticalOffset);
        }
        else {
          gDisplay.renderBitmap->DrawBitmap(ENull, aBitmap, rect, 0, aVerticalOffset);
        }
      }
      else {
        drawWidth = (remainDrawWidth > bgWidth) ? bgWidth : remainDrawWidth;
        TRect rect = TRect(0, 0, drawWidth, aBitmap->Height());

        if (aDrawTransparent) {
          gDisplay.renderBitmap->DrawBitmapTransparent(ENull, aBitmap, rect,  priorDrawWidth, aVerticalOffset);
        }
        else {
          gDisplay.renderBitmap->DrawBitmap(ENull, aBitmap, rect, priorDrawWidth, aVerticalOffset);
        }
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
