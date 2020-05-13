#include "GMainMenuPlayfield.h"
#include "GAnchorSprite.h"

const TInt16 WALKSPEED = 4 * FACTOR;

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
//  gResourceManager.LoadBitmap(TITLE_BMP, BKG_SLOT, IMAGE_ENTIRE);
//  mBackground = gResourceManager.GetBitmap(BKG_SLOT);

  mCloudsOffset = 0;
  mMountainsOffset = 0;
  mFarTreesOffset = 0;
  mNearTreesOffset = 0;
  mPathOffset = 0;


  BBitmap *bm = BBitmap::CreateBBitmap(1, 1, SCREEN_DEPTH, MEMF_FAST);

  gResourceManager.LoadBitmap(LOGO_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBgLogo = gResourceManager.GetBitmap(BKG_SLOT);
  mBgLogo->Remap(bm);

  gResourceManager.LoadBitmap(SKY_BMP, EARTH_FINAL_BOSS_PROJECTILE_SLOT, IMAGE_ENTIRE);
  mBgSky = gResourceManager.GetBitmap(EARTH_FINAL_BOSS_PROJECTILE_SLOT);
  mBgSky->Remap(bm);

  gResourceManager.LoadBitmap(FAR_TREES_BMP, FIRE_FINAL_BOSS_PROJECTILE_SLOT, IMAGE_ENTIRE);
  mBgFarTrees = gResourceManager.GetBitmap(FIRE_FINAL_BOSS_PROJECTILE_SLOT);
  mBgFarTrees->Remap(bm);

  gResourceManager.LoadBitmap(NEAR_TREES_BMP, ENERGY_FINAL_BOSS_PROJECTILE_SLOT, IMAGE_ENTIRE);
  mBgNearTrees = gResourceManager.GetBitmap(ENERGY_FINAL_BOSS_PROJECTILE_SLOT);
  mBgNearTrees->Remap(bm);

  gResourceManager.LoadBitmap(WALKING_PATH_BMP, EARTH_FINAL_BOSS_PILLAR_SLOT, IMAGE_ENTIRE);
  mBgWalkingPath = gResourceManager.GetBitmap(EARTH_FINAL_BOSS_PILLAR_SLOT);
  mBgWalkingPath->Remap(bm);

  gResourceManager.LoadBitmap(MOUNTAINS_BMP, WATER_FINAL_BOSS_PROJECTILE_SLOT, IMAGE_ENTIRE);
  mBgMountains = gResourceManager.GetBitmap(WATER_FINAL_BOSS_PROJECTILE_SLOT);
  mBgMountains->Remap(bm);

  gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);
  gResourceManager.GetBitmap(PLAYER_SLOT)->Remap(bm);

  mPlayer = new GPlayerSprite(aGameState);
  mPlayer->ClearFlags(SFLAG_RENDER_DEBUG);
//
  mPlayer->StartAnimation(walkRightAnimation);
  mPlayer->x = 10;
  mPlayer->y = SCREEN_HEIGHT - 20;

  for (int i = 0; i < bm->CountUsedColors(); ++i) {
    TRGB color = bm->GetColor(i);
    printf("%i - %02X%02X%02X\n", i, color.r, color.b, color.g);
  }


  gDisplay.SetPalette(bm->GetPalette(), 0, bm->CountUsedColors());
  mState = 0;
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
}

GMainMenuPlayfield::~GMainMenuPlayfield() {
//  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
//  mPlayer->Remove();
//  delete mPlayer;
//  mPlayer = ENull;

  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  gResourceManager.ReleaseBitmapSlot(EARTH_FINAL_BOSS_PROJECTILE_SLOT);
  gResourceManager.ReleaseBitmapSlot(FIRE_FINAL_BOSS_PROJECTILE_SLOT);
  gResourceManager.ReleaseBitmapSlot(WATER_FINAL_BOSS_PROJECTILE_SLOT);
  gResourceManager.ReleaseBitmapSlot(ENERGY_FINAL_BOSS_PROJECTILE_SLOT);
  gResourceManager.ReleaseBitmapSlot(EARTH_FINAL_BOSS_PILLAR_SLOT);

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

  mCloudsOffset += .5;
  if ((TInt) mCloudsOffset >= mBgSky->Width()) {
    mCloudsOffset = 0;
  }

//  mMountainsOffset += 0;
//  if ((TInt) mMountainsOffset >= mBgMountains->Width()) {
//    mMountainsOffset = 0;
//  }

//  mFarTreesOffset += .02;
//  if ((TInt) mFarTreesOffset >= mBgFarTrees->Width()) {
//    mFarTreesOffset = 0;
//  }

  mNearTreesOffset += .03;
  if ((TInt) mNearTreesOffset >= mBgNearTrees->Width()) {
    mNearTreesOffset = 0;
  }

  mPathOffset += .75;
  if ((TInt) mPathOffset >= mBgWalkingPath->Width()) {
    mPathOffset = 0;
  }

}


void GMainMenuPlayfield::Render() {
//  gDisplay.renderBitmap->CopyPixels(mBackground);

//  const TRGB clearColor = TRGB(0xEF, 0xF5, 0xF4);

  gDisplay.renderBitmap->Clear(9);


  const TInt walkingPathOffset = DISPLAY_HEIGHT - mBgWalkingPath->Height(),
             nearTreesOffset   = DISPLAY_HEIGHT - mBgNearTrees->Height();

  DrawScrolledBackground(mBgSky, mCloudsOffset, 0, EFalse);
  DrawScrolledBackground(mBgMountains, mMountainsOffset, 50, ETrue);
  DrawScrolledBackground(mBgFarTrees, mFarTreesOffset, 110, ETrue);
  DrawScrolledBackground(mBgNearTrees, mNearTreesOffset, nearTreesOffset, ETrue);
  DrawScrolledBackground(mBgWalkingPath, mPathOffset, walkingPathOffset, ETrue);

  gDisplay.renderBitmap->DrawBitmapTransparent(
     ENull,                      // ViewPort
     mBgLogo,                      // bitmap
     TRect(0, 0, mBgLogo->Width() - 1, mBgLogo->Height() - 1),  // src rect
     45,
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
