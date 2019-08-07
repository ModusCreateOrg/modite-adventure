#include "GGame.h"

static TUint32 start;

GGame::GGame() {
  // Load Game Options
#ifdef ENABLE_OPTIONS
  gOptions = new TOptions();
#endif

#ifdef __XTENSA__
#ifdef ENABLE_OPTIONS
  gDisplay.SetBrightness(MAX(MIN_BRIGHTNESS, MAX_BRIGHTNESS * gOptions->brightness));
#endif
#endif

  // TODO: Jay - this needs to be in BApplication constructor (I think)
#ifdef ENABLE_AUDIO
//  gSoundPlayer.Init(2 /*channels*/, 5 /*numSamples*/);
#endif

  // preload bitmaps
  for (TInt16 slot=0; slot<MAX_BBITMAP; slot++) {  // 26 is the last BMP in Resources.h (plus one)
    gResourceManager.PreloadBitmap(slot);
  }

  gResourceManager.LoadBitmap(CHARSET_8X8_BMP, FONT_8x8_SLOT, IMAGE_8x8);
  gResourceManager.CacheBitmapSlot(FONT_8x8_SLOT);
  gResourceManager.LoadBitmap(CHARSET_16X16_BMP, FONT_16x16_SLOT, IMAGE_16x16);
  gResourceManager.CacheBitmapSlot(FONT_16x16_SLOT);

  gViewPort = new BViewPort();
  gViewPort->Offset(0, 0);
  gViewPort->SetRect(TRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));

  mState = mNextState = -1;
  gGameEngine = ENull;
  SetState(GAME_STATE_SPLASH);
  start = Milliseconds();
}

GGame::~GGame() {
#ifdef ENABLE_OPTIONS
  delete gOptions;
#endif
  delete gGameEngine;
  delete gViewPort;
}

void GGame::SetState(TInt aNewState) {
  mNextState = aNewState;
}

void GGame::Run() {
#ifdef ENABLE_OPTIONS
  TBool muted = gOptions->muted;
#endif

  TBool done = EFalse;
  while (!done) {
    Random(); // randomize

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
        case GAME_STATE_GAME:
          delete gGameEngine;
          gGameEngine = new GGameState();
          break;
        default:
          continue;
      }
      // reset dKeys so next state doesn't react to any keys already pressed
      gControls.dKeys = 0;
      mState = mNextState;
    }
    gGameEngine->GameLoop();
    gDisplay.Update();
    TUint32 now = Milliseconds();
    start = now;

#ifdef FRAME_RATE_INFO
    TUint32 elapsed = now - start;
    printf("elapsed %4d\r", elapsed);
#endif

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
