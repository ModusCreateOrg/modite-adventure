#include "Game.h"
#include "GGame.h"
#include "GPlayer.h"
#include "inventory/GInventory.h"

static TUint32 start;

BFont *gFont8x8, *gFont16x16;

BViewPort gFullViewPort;

#ifdef DEBUG_MODE
TBool GGame::mDebug = ETrue;
#else
TBool GGame::mDebug = EFalse;
#endif

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

GGame::GGame() {
  mLocalData = ENull;
  mLocalDataSize = 0;

//  TRect r(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
//  gFullViewPort.SetRect(r);
  gFullViewPort.SetRect(TRect (0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));

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
  gViewPort->SetRect(TRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gViewPort->Offset(0, 0);

  mState = mNextState = -1;
  gGameEngine = ENull;
  mGameMenu = ENull;
  mDebugMenu = ENull;
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

BGameEngine *GGame::CurrentState() {
  if (gGameEngine->IsPaused()) {
    if (mGameMenu) return mGameMenu;
    else if (mInventory) return mInventory;
    else if (mDebugMenu) return mDebugMenu;
  }

  return gGameEngine;
}

void GGame::ToggleInGameMenu() {
  // TODO: @jaygarcia pause main game music and switch to pause menu specifc (if
  // need be)
  if (GPlayer::mGameOver || mDebugMenu || mInventory) {
    return;
  }
  if (mGameMenu) {
    delete mGameMenu;
    mGameMenu = ENull;
    gGameEngine->Resume();
  }
  else {
    mGameMenu = new GGameMenuState((GGameState *)gGameEngine);
    gGameEngine->Pause();
  }
  gControls.dKeys = 0;
}

void GGame::ToggleDebugMenu() {
  if (GPlayer::mGameOver || mGameMenu || mInventory) {
    return;
  }
  if (mDebugMenu) {
    delete mDebugMenu;
    mDebugMenu = ENull;
    gGameEngine->Resume();
  }
  else {
    mDebugMenu = new GDebugMenuState();
    gGameEngine->Pause();
  }
  gControls.dKeys = 0;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGame::ToggleInventory() {
  if (GPlayer::mGameOver || mGameMenu) {
    return;
  }
  if (mInventory) {
    delete mInventory;
    mInventory = ENull;
    if (!mDebugMenu) {
      gGameEngine->Resume();
    }
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

void GGame::SetState(TInt aNewState, TAny *aLocalData, TUint32 aSize) {
  mNextState = aNewState;
  if (aLocalData) {
    delete (TUint8 *)mLocalData;
    mLocalDataSize = aSize;
    mLocalData = new TUint8[mLocalDataSize];
    memcpy((TUint8 *)mLocalData, (TUint8 *)aLocalData, mLocalDataSize);
  }
  else {
    delete (TUint8 *)mLocalData;
    mLocalData = ENull;
    mLocalDataSize = 0;
  }
}

TInt GGame::GetState() {
  return mState;
}

void GGame::StartGame( char *aGameName) {
  printf("START GAME (%s)\n", aGameName);
  SetState(GAME_STATE_RESUME_GAME, aGameName, strlen(aGameName)+1);
}

TBool GGame::IsGameState() {
  TBool state = mState == GAME_STATE_RESUME_GAME || mState == GAME_STATE_GAME;
  if (!state) {
    return EFalse;
  }
  GGameState *s = (GGameState *)gGameEngine;
  return !s->IsGameOver();
}

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
    gDisplay.displayBitmap->SetColor(COLOR_SHMOO_RED, mShmoo.r, 20, 20);
    gDisplay.displayBitmap->SetColor(COLOR_SHMOO_GREEN, 20, mShmoo.g, 20);

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
        case GAME_STATE_LOAD_GAME:
          delete gGameEngine;
          gGameEngine = new GLoadGameState();
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
        case GAME_STATE_RESUME_GAME:
          delete gGameEngine;
          gGameEngine = new GGameState((char *)mLocalData);
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
    else if (mDebugMenu) {
      gGameEngine->GameLoop();
      mDebugMenu->GameLoop();
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
    if (gControls.WasPressed(CONTROL_DEBUG)) {
      if (IsGameState()) {
        ToggleDebugMenu();
      }
    }
#endif

    if (IsGameState() && gControls.WasPressed(BUTTON_START)) {
      ToggleInGameMenu();
    }

    // right shoulder button brings up inventory
    if (IsGameState() && gControls.WasPressed(CONTROL_INVENTORY)) {
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
