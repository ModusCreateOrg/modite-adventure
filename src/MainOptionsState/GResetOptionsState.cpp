#include "Game.h"
#include "GResetOptionsWidget.h"
#include "GResetAllWidget.h"
#include "GCancelResetWidget.h"

class ResetOptionsContainer : public GDialogWidget {
public:
    ResetOptionsContainer(TInt aX, TInt aY) : GDialogWidget("RESET GAME", aX, aY) {
      AddWidget((BWidget &) *new GResetOptionsWidget());
      AddWidget((BWidget &) *new GResetAllWidget());
      AddWidget((BWidget &) *new GCancelResetWidget());
    }
};

class GResetOptionsProcess : public BProcess {
public:
    GResetOptionsProcess() : BProcess() {
      mContainer = new ResetOptionsContainer(80, 92);
    }

    ~GResetOptionsProcess() {
      delete mContainer;
    }

public:
    TBool RunBefore() {
      mContainer->Render(30, 20);
      mContainer->Run();
      return ETrue;
    }

    TBool RunAfter() {
      if (gControls.WasPressed(BUTTON_MENU | BUTTON_START)) {
        gGame->SetState(GAME_STATE_MAIN_OPTIONS);
        // TODO: @jaygarcia
#ifdef ENABLE_AUDIO
        gSoundPlayer.SfxMenuCancel();
#endif
        return EFalse;
      }
      return ETrue;
    }

protected:
    ResetOptionsContainer *mContainer;
};

class GResetOptionsPlayfield : public BPlayfield {
public:
    GResetOptionsPlayfield() {
      gResourceManager.LoadBitmap(TITLE_BMP, BKG_SLOT, IMAGE_ENTIRE);
      mBackground = gResourceManager.GetBitmap(BKG_SLOT);

      gDisplay.SetPalette(mBackground);
      gDisplay.SetColor(COLOR_TEXT, WHITE);
      gDisplay.SetColor(COLOR_TEXT_SHADOW, ROSE);
      gDisplay.SetColor(COLOR_TEXT_BG, RED);

      gWidgetTheme.Configure(
        WIDGET_TEXT_BG, COLOR_TEXT_BG,
        WIDGET_TITLE_FONT, gFont16x16,
        WIDGET_TITLE_FG, COLOR_TEXT,
        WIDGET_TITLE_BG, COLOR_TEXT_SHADOW,
        WIDGET_SLIDER_FG, COLOR_TEXT_BG,
        WIDGET_SLIDER_BG, COLOR_TEXT,
        WIDGET_WINDOW_BG, gDisplay.renderBitmap->TransparentColor(),
        WIDGET_WINDOW_FG, gDisplay.renderBitmap->TransparentColor(),
        WIDGET_END_TAG
      );
    }

    virtual ~GResetOptionsPlayfield() {
      gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
    }

public:
    void Render() {
      gDisplay.renderBitmap->CopyPixels(mBackground);

      const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);
      const char* questionStr = "What would you like to reset?";
      const TInt charWidth = f->mWidth - 6;

      gDisplay.renderBitmap->DrawStringShadow(
        ENull,
        questionStr,
        f,
        (SCREEN_WIDTH - strlen(questionStr) * charWidth) / 2,
        80,
        gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_FG)),
        gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_BG)),
        -6
      );
    }

public:
    BBitmap *mBackground;
};


GResetOptionsState::GResetOptionsState() : BGameEngine(gViewPort) {
  mPlayfield = new GResetOptionsPlayfield();
  AddProcess(new GResetOptionsProcess());
}

GResetOptionsState::~GResetOptionsState() {}


