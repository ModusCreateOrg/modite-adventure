#include "GGameOver.h"
#include "GPlayer.h"

// grayscale algorithms:
// https://www.programminghomeworkhelp.com/c-programming/

GGameOver::GGameOver(GGameState *aGameState) {
  mGameState = aGameState;
  // save palette
  TRGB *source = gDisplay.renderBitmap->GetPalette();
  for (TInt color = 0; color < COLOR_TEXT; color++) {
    TRGB c = source[color];
    mSavedPalette[color] = c;
    // convert color to grayscale color
    TUint8 max = MAX(c.b, MAX(c.r, c.g));
    c.r = c.g = c.b = max;
    // set grayscale color in palette
    gDisplay.SetColor(color, c);
  }
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);

#ifdef DEBUG_MODE
  mState = 0;
#else
  mState = 1;
#endif

  // Load saved games
  gSavedGameList.LoadSavedGameList();
}

GGameOver::~GGameOver() {
  gDisplay.SetPalette(mSavedPalette);
}

static void text(const char *s, TInt x, TInt y, TBool inverse = EFalse) {
  BBitmap *b = gDisplay.renderBitmap;

  if (inverse) {
    b->DrawString(gViewPort, s, gFont16x16, x, y, COLOR_TEXT_BG, COLOR_TEXT, -4);
  }
  else {
    b->DrawString(gViewPort, s, gFont16x16, x, y, COLOR_TEXT, -1, -4);
  }
}

static void center(const char *s, TInt y, TBool inverse = EFalse) {
  BBitmap *b = gDisplay.renderBitmap;
  TInt w = strlen(s) * 12,
       x = (SCREEN_WIDTH - w) / 2;

  if (inverse) {
    b->DrawString(gViewPort, s, gFont16x16, (SCREEN_WIDTH - w) / 2, y, COLOR_TEXT_BG, COLOR_TEXT, -4);
  }
  else {
    b->DrawString(gViewPort, s, gFont16x16, (SCREEN_WIDTH - w) / 2, y, COLOR_TEXT, -1, -4);
  }
}

// return EFalse when menu is done
TBool GGameOver::Run() {
  BBitmap *b = gDisplay.renderBitmap;
  TInt x = 4, y = 10, line_height = 18;

  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);

  center("You are dead", y);
  y += line_height * 2;

#ifdef DEBUG_MODE
  center("Resume Game (debug)", y, mState == 0);
  y += line_height;
#endif

  center("Load Saved Game", y, mState == 1);
  y += line_height;

  center("Exit", y, mState == 2);

  if (gControls.WasPressed(JOYUP)) {
#ifdef DEBUG_MODE
    mState = CLAMP(mState - 1, 0, 2);
#else
    mState = CLAMP(mState - 1, 1, 2);
#endif
  }

  if (gControls.WasPressed(JOYDOWN)) {
#ifdef DEBUG_MODE
    mState = CLAMP(mState + 1, 0, 2);
#else
    mState = CLAMP(mState + 1, 1, 2);
#endif
  }

  if (gControls.WasPressed(BUTTON_SELECT | BUTTON_START | BUTTONA)) {
    // restore palette
    for (TInt color = 0; color < COLOR_TEXT; color++) {
      TRGB c = mSavedPalette[color];
      gDisplay.SetColor(color, c);
    }
    switch (mState) {
      case 0:
        mGameState->TryAgain();
        gControls.Reset();
        break;
      case 1:
        mGameState->TryAgain();
        gControls.Reset();

        if (gSavedGameList.mNumSavedGames > 0) {
          char saveGameName[256];
          gSavedGameList.First()->GameName(saveGameName);
          gGame->StartGame(saveGameName);
        } else {
          gGame->SetState(GAME_STATE_RESET_GAME);
        }
        return EFalse;
      case 2:
      default:
        gGame->SetState(GAME_STATE_MAIN_MENU);
        break;
    }
  }

  return ETrue;
}
