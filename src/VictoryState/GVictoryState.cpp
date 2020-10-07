#include "Game.h"
#include "GVictoryPlayfield.h"

class GVictoryProcess : public BProcess {
public:
  GVictoryProcess() : BProcess(0) {
//    gSoundPlayer.PlayMusic(ENDING_THEME_XM);
  }

protected:

public:
  TBool RunBefore() OVERRIDE {
//    gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
//    gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
//    CenterText16("You won!", 120, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
//    CenterText8("> QUIT", 180, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
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
