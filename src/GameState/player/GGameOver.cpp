#include "GGameOver.h"
#include "GPlayer.h"

// grayscale algorithms:
// https://www.programminghomeworkhelp.com/c-programming/

const TInt LINE_HEIGHT = 24;

class GGameOverButtonWidget : GButtonWidget {
public:
  EXPLICIT GGameOverButtonWidget(const char *aText, TInt aState = -1) : GButtonWidget(aText, aState) {};

public:
  TInt Render(TInt aX, TInt aY) OVERRIDE {
    center(mText, aY, IsActive());

    return LINE_HEIGHT;
  };
};

class GRetryButtonWidget : GGameOverButtonWidget {
public:
  GRetryButtonWidget(GGameState *aGameState, const char *aText, TBool aExitDungeon) : GGameOverButtonWidget(
    aText) {
    mGameState = aGameState;
    mExitDungeon = aExitDungeon;
  };

  void Select() OVERRIDE {
    mGameState->TryAgain(mExitDungeon);
    gControls.Reset();
  };

private:
  GGameState *mGameState;
  TBool mExitDungeon;
};

GGameOver::GGameOver(GGameState *aGameState) : GDialogWidget("", 0, 0) {
  mGameState = aGameState;
  // save palette
  TRGB *source = gDisplay.renderBitmap->GetPalette();
  for (TInt color = 0; color <= COLOR_TEXT; color++) {
    TRGB c = source[color];
    mSavedPalette[color] = c;
    // convert color to grayscale color
    TUint8 max = MAX(c.b, MAX(c.r, c.g));
//    c.r = c.g = c.b = max;
    c.r = c.g = c.b = (max > 0) ? (max * .25) : 0;

    // set grayscale color in palette
    gDisplay.SetColor(color, c);
  }
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);

  mState = 0;
  mTimer = START_DELAY;
  AddWidget((BWidget &) *new GRetryButtonWidget(mGameState, "Try Again", EFalse));
  if (mGameState->Dungeon() != OVERWORLD_DUNGEON) {
    AddWidget((BWidget &) *new GRetryButtonWidget(mGameState, "Leave Dungeon", ETrue));
  }
  AddWidget((BWidget &) *new GGameOverButtonWidget("Exit", GAME_STATE_MAIN_MENU));
}

GGameOver::~GGameOver() {
  gDisplay.SetPalette(mSavedPalette);
}

static void center(const char *s, TInt y, TBool inverse) {
  BBitmap *b = gDisplay.renderBitmap;
  TInt w = strlen(s) * 12,
       x = (SCREEN_WIDTH - w) / 2;

  if (inverse) {
    b->DrawString(gViewPort, s, gFont16x16, x, y, COLOR_TEXT_BG, COLOR_TEXT, -4);
  }
  else {
    b->DrawString(gViewPort, s, gFont16x16, x, y, COLOR_TEXT, -1, -4);
  }
}

void GGameOver::Run() {
  if (mTimer) {
    mTimer--;
    gControls.Reset();
  }

  GDialogWidget::Run();
  center("You Failed", 20);
  GDialogWidget::Render(0, 70);
}
