#include "GLoadGameState.h"
#include "GGame.h"
#include "common/GSavedGameList.h"
#include "Game.h"

const TInt BLINK_TIME = 30;

GLoadGameState::GLoadGameState() : BGameEngine(gViewPort) {
  gViewPort->SetRect(TRect(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  mPlayfield = ENull;
  Reload();
}

GLoadGameState::~GLoadGameState() = default;

void GLoadGameState::Reload() {
  gSavedGameList.LoadSavedGameList();
  if (gSavedGameList.mNumSavedGames < 1) {
    gGame->SetState(GAME_STATE_MAIN_MENU);
    return;
  }
  //  gSavedGameList.Dump();
  mCurrent = mTop = 0;
  mHeight = 5;
  mCurrentNode = ENull;
  mConfirm = EFalse;
  mBlinkTimer = 0;
}

/********************************************************************************
 ********************************************************************************
 ********************************************************************************/

TInt GLoadGameState::CenterText8(const char *s, TInt aY, TInt aColor, TInt aBackground) {
  TInt x = TInt((SCREEN_WIDTH - (strlen(s) * 8)) / 2);
  gDisplay.renderBitmap->DrawString(ENull, s, gFont8x8, x, aY, aColor, aBackground);
  return 8;
}

TInt GLoadGameState::CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground) {
  TInt x = TInt((SCREEN_WIDTH - StringLength16(s)) / 2);
  gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x, aY, aColor, TInt16(aBackground), -4);
  return 16;
}

TInt GLoadGameState::CenterTextShadow16(const char *s, TInt aY, TInt aColor, TInt aBackground, TInt aShadow) {
  TInt x = TInt((SCREEN_WIDTH - StringLength16(s)) / 2);
  gDisplay.renderBitmap->DrawStringShadow(ENull, s, gFont16x16, x, aY, aColor, TInt16(aShadow), TInt16(aBackground),
    -4);
  return 16;
}

TInt GLoadGameState::Text8(const char *s, TInt aX, TInt aY, TInt aColor, TInt aBackground) {
  gDisplay.renderBitmap->DrawString(ENull, s, gFont8x8, aX, aY, aColor, TInt16(aBackground), 0);
  return 10;
}

TInt GLoadGameState::Text16(const char *s, TInt aX, TInt aY, TInt aColor, TInt aBackground) {
  gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, aX, aY, aColor, TInt16(aBackground), -4);
  return 16;
}

/********************************************************************************
 ********************************************************************************
 ********************************************************************************/

void GLoadGameState::PreRender() {
  gDisplay.renderBitmap->Clear(COLOR_TEXT_BG);
  gDisplay.renderBitmap->SetColor(COLOR_SHMOO_RED, 255, 0, 0);

  if (mConfirm) {
    gDisplay.renderBitmap->SetColor(COLOR_TEXT, 128, 128, 128);
  }
  else {
    gDisplay.renderBitmap->SetColor(COLOR_TEXT, 255, 255, 255);
  }

  gDisplay.renderBitmap->SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.renderBitmap->SetColor(COLOR_TEXT_SHADOW, 255, 0, 0);
  TInt x = 80, y = 10;
  y += CenterTextShadow16("Saved Games", y, COLOR_TEXT, -1, COLOR_TEXT_SHADOW) + 10;
  mHeight = 0;
  for (GSavedGameNode *n = gSavedGameList.First(); !gSavedGameList.End(n); n = gSavedGameList.Next(n)) {
    if (mHeight < mTop) {
      mHeight++;
      continue;
    }
    if (y > 150) {
      break;
    }
    if (mCurrent == mHeight) {
      mCurrentNode = n;
      y += Text16(n->mDisplayName, x, y, COLOR_TEXT_BG, COLOR_TEXT);
    }
    else {
      y += Text16(n->mDisplayName, x, y, COLOR_TEXT, COLOR_TEXT_BG);
    }
    mHeight++;
  }
  y = 180;
  x = 40;
  if (mConfirm) {
    mBlinkTimer--;
    if (mBlinkTimer < 1) {
      mBlinkState = !mBlinkState;
      mBlinkTimer = BLINK_TIME;
    }
    if (mBlinkState) {
      y += Text16("START:  Delete Game", x, y, COLOR_SHMOO_RED, COLOR_TEXT_BG);
      Text16("SELECT: Cancel", x, y, COLOR_SHMOO_RED, COLOR_TEXT_BG);
    }
  }
  else {
    y += Text16("START:  Load game", x, y, COLOR_TEXT, COLOR_TEXT_BG);
    y += Text16("SELECT: Delete game", x, y, COLOR_TEXT, COLOR_TEXT_BG);
    Text16("LEFT:   Cancel", x, y, COLOR_TEXT, COLOR_TEXT_BG);
  }
}

void GLoadGameState::PostRender() {
  if (!mConfirm) {

    if (gControls.WasPressed(JOYUP)) {
      if (mCurrent) {
        if (mCurrent == mTop && mTop > 0) {
          mTop--;
        }
        mCurrent--;
      }
    }

    if (gControls.WasPressed(JOYDOWN)) {
      if (mCurrent < gSavedGameList.mNumSavedGames - 1) {
        mCurrent++;
        if (mCurrent >= mHeight) {
          mTop++;
        }
      }
    }

    if (gControls.WasPressed(JOYLEFT)) {
      gGame->SetState(GAME_STATE_MAIN_MENU);
      return;
    }

    if (gControls.WasPressed(BUTTON_START)) {
      // select, load it
//      printf("START!\n");
      char name[256];
      mCurrentNode->GameName(name);
      gGame->StartGame(name);
      return;
    }
  }

  if (gControls.WasPressed(BUTTON_SELECT)) {
    // delete
    mConfirm = !mConfirm;
    mBlinkTimer = BLINK_TIME;
  }

  if (mConfirm) {
    if (gControls.WasPressed(BUTTON_START)) {
      printf("DELETE %s (%s)\n", mCurrentNode->mDisplayName, mCurrentNode->mFilename);
      gSavedGameList.RemoveGame(mCurrentNode);
      Reload();
    }
    else if (gControls.WasPressed(BUTTONA)) {
      mConfirm = EFalse;
    }
  }
}
