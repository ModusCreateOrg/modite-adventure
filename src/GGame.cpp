#include "GGame.h"

static TUint32 start;

BFont *gFont8x8, *gFont16x16;

BViewPort gFullViewPort;

#ifdef DEBUG_MODE
TBool GGame::mDebug = ETrue;
#endif

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

GGame::GGame() {
  TRect r(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
  gFullViewPort.SetRect(r);

  // Load Game Options
#ifdef ENABLE_OPTIONS
  gOptions = new TOptions();
#endif

#ifdef __XTENSA__
#ifdef ENABLE_OPTIONS
  gDisplay.SetBrightness(
      MAX(MIN_BRIGHTNESS, MAX_BRIGHTNESS * gOptions->brightness));
#endif
#endif

  // TODO: Jay - this needs to be in BApplication constructor (I think)
#ifdef ENABLE_AUDIO
  gSoundPlayer.Init(5 /*channels*/);
#endif

  // preload bitmaps
  // MAX_BITMAP is defined in GResource.h.
  //  for (TInt16 slot = 0; slot <= MAX_BBITMAP; slot++) {
  //    gResourceManager.PreloadBitmap(slot);
  //  }

  gResourceManager.LoadBitmap(CHARSET_8X8_BMP, FONT_8x8_SLOT, IMAGE_8x8);
  gResourceManager.CacheBitmapSlot(FONT_8x8_SLOT);
  gFont8x8 = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_8x8);
  gResourceManager.LoadBitmap(CHARSET_16X16_BMP, FONT_16x16_SLOT, IMAGE_16x16);
  gResourceManager.CacheBitmapSlot(FONT_16x16_SLOT);
  gFont16x16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);

  gViewPort = new BViewPort();
  // TODO @michaeltintiuc - the sprites are clipped at the bottom SCREEN_HEIGHT-16  (BUG)
  gViewPort->SetRect(TRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gViewPort->Offset(0, 0);

  mState = mNextState = -1;
  gGameEngine = ENull;
  mGameMenu = ENull;
  mInventory = ENull;
  SetState(GAME_STATE_SPLASH);
  start = Milliseconds();
  mShmoo.Set(0, 0, 0);
}

GGame::~GGame() {
#ifdef ENABLE_OPTIONS
  delete gOptions;
#endif
  delete gGameEngine;
  delete gViewPort;
  delete mGameMenu;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGame::ToggleInGameMenu() {
  // TODO: @jaygarcia pause main game music and switch to pause menu specifc (if
  // need be)
  if (mGameMenu) {
    delete mGameMenu;
    mGameMenu = ENull;
    gGameEngine->Resume();
  }
  else {
    mGameMenu = new GGameMenuState();
    gGameEngine->Pause();
  }
  gControls.dKeys = 0;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGame::ToggleInventory() {
  if (mInventory) {
    delete mInventory;
    mInventory = ENull;
    gGameEngine->Resume();
  }
  else {
    mInventory = new GInventory(&gFullViewPort);
    gGameEngine->Pause();
  }
  gControls.dKeys = 0;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGame::SetState(TInt aNewState) { mNextState = aNewState; }

TInt GGame::GetState() { return mState; }

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGame::Run() {
#ifdef ENABLE_OPTIONS
  TBool muted = gOptions->muted;
#endif

  TBool done = EFalse;
  while (!done) {
    Random(); // randomize
    mShmoo.Set(TUint8(mShmoo.r + 16), TUint8(mShmoo.g + 16), TUint8(mShmoo.b + 16));
    gDisplay.displayBitmap->SetColor(COLOR_SHMOO, mShmoo);

    if (mNextState != mState) {
      switch (mNextState) {
        case GAME_STATE_SPLASH:
          delete gGameEngine;
          gGameEngine = new GSplashState();
          break;
        case GAME_STATE_MAIN_MENU:
          delete gGameEngine;
          gGameEngine = new GMainMenuState();
          break;
        case GAME_STATE_MAIN_OPTIONS:
          delete gGameEngine;
          gGameEngine = new GMainOptionsState();
          break;
        case GAME_STATE_RESET_OPTIONS:
          delete gGameEngine;
          gGameEngine = new GResetOptionsState();
          break;
        case GAME_STATE_GAME:
          delete gGameEngine;
          gGameEngine = new GGameState();
          break;
        case GAME_STATE_CREDITS:
          delete gGameEngine;
          gGameEngine = new GCreditsState();
          break;
        default:
          continue;
      }
      // reset dKeys so next state doesn't react to any keys already pressed
      gControls.dKeys = 0;
      mState = mNextState;
    }

    if (mInventory) {
      mInventory->GameLoop();
    }
    else if (mGameMenu) {
      gGameEngine->GameLoop();
      mGameMenu->GameLoop();
    }
    else {
      gGameEngine->GameLoop();
    }

    gDisplay.Update();
    TUint32 now = Milliseconds();
    start = now;

#ifdef FRAME_RATE_INFO
    TUint32 elapsed = now - start;
    printf("elapsed %4d\r", elapsed);
#endif

#ifdef DEBUG_MODE
    if (gControls.WasPressed(BUTTON_MENU)) {
      mDebug = !mDebug;
      printf("DEBUGING %s\n", mDebug ? "ENABLED" : "DISABLED");
    }
#endif

    if (gControls.WasPressed(BUTTON_START) && mState == GAME_STATE_GAME) {
      ToggleInGameMenu();
    }

    // right shoulder button brings up inventory
    if (gControls.WasPressed(BUTTONR) && mState == GAME_STATE_GAME) {
      ToggleInventory();
    }

    if (gControls.WasPressed(BUTTONQ)) {
      done = true;
    }
#ifdef ENABLE_OPTIONS
    if (gControls.WasPressed(BUTTON2)) {
      muted = !muted;
      gOptions->muted = muted;
      gOptions->Save();
#ifdef ENABLE_AUDIO
      //      gSoundPlayer.MuteMusic(muted);
#endif
    }
#endif
  }
}
