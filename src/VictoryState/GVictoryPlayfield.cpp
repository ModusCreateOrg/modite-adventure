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
  mCreditsProcess = new GVictoryCreditsProcess(COLOR_TEXT);
  mBgColors = new TRGB[256];
  mFadePct = 0;
  mFadeStep = .001;
//  mFadeStep = .01;

  mStateTimer = 0;
  mTimer = 120;
  mState = STATE_START;

  // ** Load Bitmaps and cache their colors for a later fade in **//
  BBitmap *bm = BBitmap::CreateBBitmap(1, 1, SCREEN_DEPTH, MEMF_FAST);

//  gResourceManager.LoadBitmap(CLOUDS_TOP_BMP, MAIN_MENU_SLOT0, IMAGE_ENTIRE);
//  mBgSky = gResourceManager.GetBitmap(MAIN_MENU_SLOT0);
//  mBgSky->Remap(bm);

  gResourceManager.LoadBitmap(NEAR_TREES_BMP, MAIN_MENU_SLOT1, IMAGE_ENTIRE);
  mBgNearTrees = gResourceManager.GetBitmap(MAIN_MENU_SLOT1);
  mBgNearTrees->Remap(bm);

  gResourceManager.LoadBitmap(WALKING_PATH_BMP, MAIN_MENU_SLOT2, IMAGE_ENTIRE);
  mBgWalkingPath = gResourceManager.GetBitmap(MAIN_MENU_SLOT2);
  mBgWalkingPath->Remap(bm);

  gResourceManager.LoadBitmap(MOUNTAINS_BMP, MAIN_MENU_SLOT3, IMAGE_ENTIRE);
  mBgMountains = gResourceManager.GetBitmap(MAIN_MENU_SLOT3);
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

  /** Set Sky Color **/

  mSkyColorIndex = 200;
  TRGB *sourcePalette = bm->GetPalette();

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
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT0);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT1);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT2);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT3);
  gResourceManager.ReleaseBitmapSlot(MAIN_MENU_SLOT4);

  delete mPlayer;
  mPlayer = ENull;
  delete[] mBgColors;
  mBgColors = ENull;
}


void GVictoryPlayfield::Animate() {
  mPlayer->Animate();

  const TFloat pathSpeed = .75,
               nearTreesSpeed = pathSpeed - .70;

//  mSkyOffset += .009;
//  if ((TInt) mSkyOffset >= mBgSky->Width()) {
//    mSkyOffset = 0;
//  }

  if ((TInt) mMoonOffset > 10) {
    mMoonOffset -= .05;
  }
  else {
    moonHitTop = ETrue;
  }

  mNearTreesOffset += nearTreesSpeed;
  if ((TInt)mNearTreesOffset >= mBgNearTrees->Width()) {
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
}

void GVictoryPlayfield::RenderAnimatedBackground() {
  mStateTimer++;

  mStarfieldProcess->Render();

  const TInt walkingPathOffset = DISPLAY_HEIGHT - mBgWalkingPath->Height();

//  DrawScrolledBackground(mBgSky, mSkyOffset, 0);
  DrawScrolledBackground(mBgMountains, mMountainsOffset, 70);
  DrawScrolledBackground(mBgNearTrees, mNearTreesOffset, 80);
  DrawScrolledBackground(mBgWalkingPath, mPathOffset, walkingPathOffset);

  if (mState == STATE_RUN) {
    mCreditsProcess->Run();
  }

  mPlayer->Render(gViewPort);
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
    mFadeStep = .0001; // Slower Fade
    mStateTimer = 0;
    mState = STATE_RUN;
    mTimer = 0;
  }
  else {
    FadeInColors();
  }

  RenderAnimatedBackground();
//
//  mTimer--;
//  if (mTimer < 0) {
//    mStateTimer = 0;
//    mState = STATE_RUN;
//  }

}

void GVictoryPlayfield::StartState() {
  mTimer--;
  if (mTimer < 0) {
    mTimer = 120;
    mStateTimer = 0;
    mState = STATE_FADEIN;
  }
}

void GVictoryPlayfield::Render() {
  gDisplay.renderBitmap->Clear(mSkyColorIndex);

  switch (mState) {
    case STATE_START:
      return StartState();
    case STATE_FADEIN:
      return FadeInState();
    case STATE_RUN:
      return  RunState();
    case STATE_HOLD:
      return HoldState();
    default:
      Panic("TextProcess invalid state");
  }

//  TRect rect = TRect(0, 0, mBgMoon->Width(), mBgMoon->Height());
//  const TInt moonX = SCREEN_WIDTH - mBgMoon->Width() - 20;
//  gDisplay.renderBitmap->DrawBitmap(ENull, mBgMoon, rect, moonX, (TInt)mMoonOffset);

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
