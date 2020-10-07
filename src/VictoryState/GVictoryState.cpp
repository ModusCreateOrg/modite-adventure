#include "Game.h"
#include "GVictoryPlayfield.h"

class GVictoryProcess : public BProcess {
public:
  GVictoryProcess() : BProcess(0) {
//    gSoundPlayer.PlayMusic(ENDING_THEME_XM);
  }

protected:
  TInt CenterText8(const char *s, TInt aY, TInt aColor, TInt aBackground) {
    TInt x = TInt((SCREEN_WIDTH - (strlen(s) * 8)) / 2);
    gDisplay.renderBitmap->DrawString(ENull, s, gFont8x8, x, aY, aColor, aBackground);
    return 8;
  }

  TInt CenterText16(const char *s, TInt aY, TInt aColor, TInt aBackground) {
    TInt width = aBackground == -1 ? 12 : 16;
    TInt x = TInt((SCREEN_WIDTH - (strlen(s) * width)) / 2);
    if (aBackground != -1) {
      gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x, aY, aColor, TInt16(aBackground));
    }
    else {
      gDisplay.renderBitmap->DrawString(ENull, s, gFont16x16, x, aY, aColor, TInt16(aBackground), -4);
    }
    return 16;
  }

public:
  TBool RunBefore() OVERRIDE {
    gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
    gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
    CenterText16("You won!", 120, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
    CenterText8("> QUIT", 180, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
    return ETrue;
  }
  TBool RunAfter() OVERRIDE {
    if (gControls.WasPressed(BUTTON_SELECT | BUTTONA)) {
      gGame->SetState(GAME_STATE_MAIN_MENU);
    }
    return ETrue;
  }
};

GVictoryState::GVictoryState(GGameState *aGameEngine) : BGameEngine(gViewPort) {
  mPlayfield = new GVictoryPlayfield(aGameEngine);
  AddProcess(new GVictoryProcess());
}

GVictoryState::~GVictoryState() = default;
