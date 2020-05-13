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
  mFont8 = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_8x8);
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);


  BBitmap *bm = BBitmap::CreateBBitmap(1, 1, SCREEN_DEPTH, MEMF_FAST);

  gResourceManager.LoadBitmap(LOGO_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBgLogo = gResourceManager.GetBitmap(BKG_SLOT);
  mBgLogo->Remap(bm);

  gResourceManager.LoadBitmap(SKY_BMP, PLAYER_SLOT + 1, IMAGE_ENTIRE);
  mBgSky = gResourceManager.GetBitmap(PLAYER_SLOT + 1);
  mBgSky->Remap(bm);


  gResourceManager.LoadBitmap(NEAR_TREES_BMP, PLAYER_SLOT + 2, IMAGE_ENTIRE);
  mBgNearTrees = gResourceManager.GetBitmap(PLAYER_SLOT + 2);
  mBgNearTrees->Remap(bm);

  gResourceManager.LoadBitmap(WALKING_PATH_BMP, PLAYER_SLOT + 3, IMAGE_ENTIRE);
  mBgWalkingPath = gResourceManager.GetBitmap(PLAYER_SLOT + 3);
  mBgWalkingPath->Remap(bm);

  gResourceManager.LoadBitmap(MOUNTAINS_BMP, PLAYER_SLOT + 4, IMAGE_ENTIRE);
  mBgMountains = gResourceManager.GetBitmap(PLAYER_SLOT + 4);
  mBgMountains->Remap(bm);

  gResourceManager.LoadBitmap(MOON_WITH_LOGO_BMP, PLAYER_SLOT + 5, IMAGE_ENTIRE);
  mBgMoon = gResourceManager.GetBitmap(PLAYER_SLOT + 5);
  mBgMoon->Remap(bm);
  mMoonOffset = SCREEN_WIDTH - mBgMoon->Width() - 10;

  gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);
  gResourceManager.GetBitmap(PLAYER_SLOT)->Remap(bm);

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

  gDisplay.SetPalette(bm->GetPalette(), 0, bm->CountUsedColors());
  mState = 0;

  gDisplay.SetColor(200, 0x45, 0x53, 0x77); // Dark blue bg
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_SHADOW, 60,60,60);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
}

GMainMenuPlayfield::~GMainMenuPlayfield() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT + 1);
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT + 2);
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT + 3);
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT + 4);
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT + 5);

  delete mPlayer;
  mPlayer = ENull;

  delete mFont8;
  delete mFont16;
}

TInt GMainMenuPlayfield::CenterText8(const char *s, TInt aY, TInt aColor, TInt aBackground) {
  TInt x = TInt((SCREEN_WIDTH - (strlen(s) * 8)) / 2);
  gDisplay.renderBitmap->DrawString(ENull, s, mFont8, x, aY, aColor, aBackground);
  return 8;
}

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

//  mMountainsOffset += 0;
//  if ((TInt) mMountainsOffset >= mBgMountains->Width()) {
//    mMountainsOffset = 0;
//  }


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

}


void GMainMenuPlayfield::Render() {
  // Paint screen in dark blue
//  TRGB color = TRGB(0x45, 0x53, 0x77);
//  const TInt colorIndex = gDisplay.renderBitmap->FindColor(color);
//  printf("COLOR Index %i\n", colorIndex);
  gDisplay.renderBitmap->Clear(200);

  TRect rect = TRect(0, 0, mBgMoon->Width(), mBgMoon->Height());

  const TInt moonX  = SCREEN_WIDTH - mBgMoon->Width() - 20;
  gDisplay.renderBitmap->DrawBitmap(ENull, mBgMoon, rect, moonX, (TInt)mMoonOffset);

  const TInt walkingPathOffset = DISPLAY_HEIGHT - mBgWalkingPath->Height(),
             nearTreesOffset   = DISPLAY_HEIGHT - mBgNearTrees->Height();

  DrawScrolledBackground(mBgSky, mSkyOffset, 0, ETrue);
  DrawScrolledBackground(mBgMountains, mMountainsOffset, 50, ETrue);
  DrawScrolledBackground(mBgNearTrees, mNearTreesOffset, nearTreesOffset, ETrue);
  DrawScrolledBackground(mBgWalkingPath, mPathOffset, walkingPathOffset, ETrue);

  gDisplay.renderBitmap->DrawBitmapTransparent(
     ENull,                      // ViewPort
     mBgLogo,                      // bitmap
     TRect(0, 0, mBgLogo->Width() - 1, mBgLogo->Height() - 1),  // src rect
     50,
     10// destination point
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
