//
// Created by jg on 10/6/20.
//

#include "GVictoryPlayfield.h"
#include "GAnchorSprite.h"

const TInt16 WALK_SPEED = 10 * FACTOR;

static TBool sunHitTop = EFalse;

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


const TUint16 HOLD_SPEED = 7 * FACTOR;
ANIMSCRIPT victoryAnimation[] = {
  ABITMAP(PLAYER_SLOT),
  ADELTA(1, 0),
  ASTEP(HOLD_SPEED, 39),
  ADELTA(0, 0),
  ASTEP(HOLD_SPEED, 35),
  ADELTA(-1, 0),
  ASTEP(HOLD_SPEED, 47),
  ALABEL,
  ADELTA(0, 0),
  ASTEP(HOLD_SPEED, 3),
  ALOOP
};


GVictoryPlayfield::GVictoryPlayfield(GGameState *aGameState) {
  mGameState = aGameState;
  sunHitTop = EFalse;
  mFrame = 0;
  mStarfieldProcess = new GStarFieldProcess();
  mCreditsProcess = new GVictoryCreditsProcess(COLOR_TEXT);
  mBgColors = new TRGB[256];
  mFadePct = 0;
  mFadeStep = .0008;
//  mFadeStep = .01;

  mStateTimer = 0;
  mTimer = 120;
  mState = STATE_START;

  // ** Load Bitmaps and cache their colors for a later fade in **//
  BBitmap *bm = BBitmap::CreateBBitmap(1, 1, SCREEN_DEPTH, MEMF_FAST);

//  gResourceManager.LoadBitmap(CLOUDS_TOP_BMP, MAIN_MENU_SLOT0, IMAGE_ENTIRE);
//  mBgSky = gResourceManager.GetBitmap(MAIN_MENU_SLOT0);
//  mBgSky->Remap(bm);

  gResourceManager.ReleaseBitmapSlot(ENVIRONMENT_SLOT);
  gResourceManager.LoadBitmap(MODUS_LABS_LOGO_WHITE_BMP, ENVIRONMENT_SLOT, IMAGE_ENTIRE);
  mBgLabsLogo = gResourceManager.GetBitmap(ENVIRONMENT_SLOT);
  mBgLabsLogo->Remap(bm);


  gResourceManager.LoadBitmap(NEAR_TREES_BMP, MAIN_MENU_SLOT1, IMAGE_ENTIRE);
  mBgNearTrees = gResourceManager.GetBitmap(MAIN_MENU_SLOT1);
  mBgNearTrees->Remap(bm);

  gResourceManager.LoadBitmap(WALKING_PATH_BMP, MAIN_MENU_SLOT2, IMAGE_ENTIRE);
  mBgWalkingPath = gResourceManager.GetBitmap(MAIN_MENU_SLOT2);
  mBgWalkingPath->Remap(bm);

  gResourceManager.LoadBitmap(MOUNTAINS_BMP, MAIN_MENU_SLOT3, IMAGE_ENTIRE);
  mBgMountains = gResourceManager.GetBitmap(MAIN_MENU_SLOT3);
  mBgMountains->Remap(bm);


  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT4);
  gResourceManager.LoadBitmap(SUN_WITH_LOGO_BMP, MAIN_MENU_SLOT4, IMAGE_ENTIRE);
  mBgRisingSun = gResourceManager.GetBitmap(MAIN_MENU_SLOT4);
  mBgRisingSun->Remap(bm);

  gResourceManager.LoadBitmap(MOON_WITH_LOGO_ENDING_BMP, MAIN_MENU_SLOT5, IMAGE_ENTIRE);
  mBgMoon = gResourceManager.GetBitmap(MAIN_MENU_SLOT5);
  mBgMoon->Remap(bm);
  mMoonOffset = 10;


  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);
  gResourceManager.GetBitmap(PLAYER_SLOT)->Remap(bm);

  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  gResourceManager.LoadBitmap(LOGO_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mGameLogo = gResourceManager.GetBitmap(BKG_SLOT);


  mGameLogo->Remap(bm);

  gViewPort->mWorldX = 0;
  gViewPort->mWorldY = 0;
  gViewPort->Offset(0, 0);

  mPlayer = new GPlayerSprite(aGameState);
  mPlayer->ClearFlags(SFLAG_RENDER_DEBUG);
  mPlayer->StartAnimation(walkAnimation);
  mPlayer->x = -70;
  mPlayer->y = SCREEN_HEIGHT - 20;

  mSkyOffset = 0;
  mMountainsOffset = 10;
  mSunOffset = 135;
  mNearTreesOffset = 0;
  mPathOffset = 0;

  /** Set Sky Color **/
  mSkyColorIndex = 200;
  TRGB *sourcePalette = bm->GetPalette();

  // Find the Modus Labs logo foreground color (0x00FFFF) and cache the
  // index so we can fade it in and out.
  TUint8 found = 0;
  for (int i = 0; i < bm->CountUsedColors(); ++i) {
    TRGB srcColor = bm->GetColor(i);
    if (srcColor.r == 0x00 && srcColor.g == 0xFF && srcColor.b == 0xFF) {
      mCreditsProcess->SetLabsForegroundIndex(i);
      gDisplay.SetColor(i, 0xFF, 0xFF, 0xFF); // Force to white
      sourcePalette[i].Set(0xFF, 0xFF, 0xFF);

      found++;
    }
    if (srcColor.r == 0x00 && srcColor.g == 0xFF && srcColor.b == 0xFE) {
      mCreditsProcess->SetLabsBackgroundIndex(i);
      sourcePalette[i].Set(0x40, 0x40, 0x40); // Force to dark gray
      found++;
    }

    if (found > 1) {
      break;
    }
  }

  // Find the Game logo colors(0x00FFFF) and cache the
  // indices so we can fade it in and out.
  for (TInt i = 0; i < bm->CountUsedColors(); ++i) {
    TRGB srcColor = bm->GetColor(i);
    if (srcColor.r == 247 && srcColor.g == 181 && srcColor.b == 62) {
      mCreditsProcess->mLogoColorIndices[0] = i;
      mCreditsProcess->mLogoColors[0] = srcColor;
    }
     if (srcColor.r == 164 && srcColor.g == 105 && srcColor.b == 34) {
       mCreditsProcess->mLogoColorIndices[1] = i;
       mCreditsProcess->mLogoColors[1] = srcColor;
     }
     if (srcColor.r == 243 && srcColor.g == 112 && srcColor.b == 94) {
       mCreditsProcess->mLogoColorIndices[2] = i;
       mCreditsProcess->mLogoColors[2] = srcColor;
     }
     if (srcColor.r == 132 && srcColor.g == 35 && srcColor.b == 69) {
       mCreditsProcess->mLogoColorIndices[3] = i;
       mCreditsProcess->mLogoColors[3] = srcColor;
     }
  }



  // Set palette colors initially
  sourcePalette[mSkyColorIndex].Set(0x66, 0x99, 0xCC);
  sourcePalette[COLOR_TEXT].Set(255, 255, 255);
  sourcePalette[COLOR_TEXT_SHADOW].Set(60, 60, 60);
  sourcePalette[COLOR_TEXT_BG].Set(255, 92, 93);
//  mStarsColor.Set(TUint8(200), TUint8(200), TUint8(200));
  sourcePalette[COLOR_EXPERIENCE].Set(255, 255, 255);

  gDisplay.SetPalette(sourcePalette, 0, 256);
  CacheColors();
}

GVictoryPlayfield::~GVictoryPlayfield() {
  gResourceManager.ReleaseBitmapSlot(ENVIRONMENT_SLOT);
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT0);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT1);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT2);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT3);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT4);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT5);
  delete mPlayer;
  mPlayer = ENull;
  delete[] mBgColors;
  mBgColors = ENull;
}


void GVictoryPlayfield::Animate() {

  mPlayer->Animate();


  if (mState == STATE_HOLD) {
    return;
  }
  mStarfieldProcess->Animate();

  if (mCreditsProcess->mAnimateCharacterIn == ETrue) {
    mPlayer->x += .033;
  }

  if (mPlayer->x > -20) {
    mMoonOffset += .05;
  }

  if (mPlayer->x > 144) {
    mPlayer->x = 144;
  }

  const TFloat pathSpeed = .75,
               nearTreesSpeed = pathSpeed - .70;

//  mSkyOffset += .009;
//  if ((TInt) mSkyOffset >= mBgSky->Width()) {
//    mSkyOffset = 0;
//  }

  if (mState == STATE_FADEIN_FINAL) {
    if ((TInt) mSunOffset > 10) {
      mSunOffset -= .05;
    }
    else {
      sunHitTop = ETrue;
    }
  }

  mNearTreesOffset += nearTreesSpeed;
  if ((TInt)mNearTreesOffset >= mBgNearTrees->Width()) {
    mNearTreesOffset = 0;
  }

  mPathOffset += pathSpeed;
  if ((TInt)mPathOffset >= mBgWalkingPath->Width()) {
    mPathOffset = 0;
  }
//
//  mMountainsOffset += .001;
//  if ((TInt)mMountainsOffset >= mBgMountains->Width()) {
//    mMountainsOffset = 0;
//  }
}

void GVictoryPlayfield::RenderAnimatedBackground() {
  mStateTimer++;
  mStarfieldProcess->Render();


  if ((TInt) mMoonOffset < 160) {
    TRect moonRect = TRect(0, 0, mBgMoon->Width(), mBgMoon->Height());

    const TInt moonX = SCREEN_WIDTH - mBgMoon->Width() - 20;
    gDisplay.renderBitmap->DrawBitmapTransparent(ENull, mBgMoon, moonRect, moonX, (TInt) mMoonOffset);
  }


  TRect rect = TRect(0, 0, mBgRisingSun->Width(), mBgRisingSun->Height());
  const TInt sunX = SCREEN_WIDTH - mBgRisingSun->Width() - 30;
  gDisplay.renderBitmap->DrawBitmapTransparent(ENull, mBgRisingSun, rect, sunX, mSunOffset);

  const TInt walkingPathOffset = DISPLAY_HEIGHT - mBgWalkingPath->Height();

//  DrawScrolledBackground(mBgSky, mSkyOffset, 0);
  DrawScrolledBackground(mBgMountains, mMountainsOffset, 70);
  DrawScrolledBackground(mBgNearTrees, mNearTreesOffset, 80);
  DrawScrolledBackground(mBgWalkingPath, mPathOffset, walkingPathOffset);

  if (mState == STATE_RUN  || mState == STATE_FADEIN_FINAL) {
    mCreditsProcess->Run();
  }
//  if (mCreditsProcess->mAnimateCharacterIn == ETrue) {
    mPlayer->Render(gViewPort);
//  }
}



void GVictoryPlayfield::HoldState() {
  RenderAnimatedBackground();
}

void GVictoryPlayfield::RunState() {
  RenderAnimatedBackground();
}

void GVictoryPlayfield::FadeInState() {

  mFadePct += mFadeStep;
  if (mFadePct > .4) {
    mFadePct = .4;
    mFadeStep = .00021; // Slower Fade
    mStateTimer = 0;
    mState = STATE_RUN;
    mTimer = 0;
  }
  else {
    FadeColors();
  }

  RenderAnimatedBackground();
}

void GVictoryPlayfield::FadeInFinalState() {

  mFadePct += mFadeStep;

  if (mFadePct > 1) {
    mFadePct = 1;
    mStateTimer = 0;
    mState = STATE_HOLD;
    mTimer = 0;
    mPlayer->StartAnimation(victoryAnimation);
  }
  else {
    FadeColors();
  }

  RenderAnimatedBackground();
}

void GVictoryPlayfield::StartState() {
  mTimer--;
  if (mTimer < 0) {
    mTimer = 120;
    mStateTimer = 0;
    mState = STATE_FADEIN_INITIAL;
  }
}

void GVictoryPlayfield::Render() {
  mFrame++;

  gDisplay.renderBitmap->Clear(mSkyColorIndex);
  if (mCreditsProcess->GetText() == 9) {
    mState = STATE_FADEIN_FINAL;
    mStarfieldProcess->StopSpawn();
  }

  if (mFrame == 8000) {
    gGame->SetState(GAME_STATE_SPLASH);
  }


  switch (mState) {
    case STATE_START:
      return StartState();
    case STATE_FADEIN_INITIAL:
      return FadeInState();
    case STATE_RUN:
      return  RunState();
    case STATE_FADEIN_FINAL:
      return FadeInFinalState();
    case STATE_HOLD:
      return HoldState();
    default:
      Panic("TextProcess invalid state");
  }

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
