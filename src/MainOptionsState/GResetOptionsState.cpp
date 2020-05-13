#include "Game.h"
#include "GResetOptionsWidget.h"
#include "GResetAllWidget.h"
#include "GCancelResetWidget.h"

class ResetOptionsContainer : public GDialogWidget {
public:
    ResetOptionsContainer(TInt aX, TInt aY) : GDialogWidget("Reset Game", aX, aY) {
      AddWidget((BWidget &) *new GResetOptionsWidget());
      AddWidget((BWidget &) *new GResetAllWidget());
      AddWidget((BWidget &) *new GCancelResetWidget());
    }
};

class GResetOptionsProcess : public BProcess {
public:
    GResetOptionsProcess() : BProcess() {
      mContainer = new ResetOptionsContainer(0, 0);
    }

    ~GResetOptionsProcess() {
      delete mContainer;
    }

public:
    TBool RunBefore() {
      return ETrue;
    }

    TBool RunAfter() {
      mContainer->Render(100, 125);
      mContainer->Run();
      if (gControls.WasPressed(BUTTON_MENU | BUTTON_START)) {
        gGame->SetState(GAME_STATE_MAIN_OPTIONS);

#ifdef ENABLE_AUDIO
        gSoundPlayer.SfxMenuOut();
#endif
        return EFalse;
      }
      return ETrue;
    }

protected:
    ResetOptionsContainer *mContainer;
    BFont *mFont16;
};

class GResetOptionsPlayfield : public BPlayfield {
public:
    GResetOptionsPlayfield() {
      gResourceManager.LoadBitmap(TITLE_BMP, BKG_SLOT, IMAGE_ENTIRE);
      mBackground = gResourceManager.GetBitmap(BKG_SLOT);
      gDisplay.SetPalette(mBackground);
    }

    virtual ~GResetOptionsPlayfield() {
      gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
    }

public:
    void Render() {
      gDisplay.renderBitmap->CopyPixels(mBackground);
    }

public:
    BBitmap *mBackground;
};


GResetOptionsState::GResetOptionsState() : BGameEngine(gViewPort) {
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
  mPlayfield = new GResetOptionsPlayfield();
  AddProcess(new GResetOptionsProcess());

  gWidgetTheme.Configure(
      WIDGET_TEXT_FONT, mFont16,
      WIDGET_TEXT_FG, COLOR_TEXT,
      WIDGET_TEXT_BG, COLOR_TEXT_BG,
      WIDGET_TITLE_FONT, mFont16,
      WIDGET_TITLE_FG, COLOR_TEXT,
      WIDGET_TITLE_BG, -1,
      WIDGET_WINDOW_BG, gDisplay.renderBitmap->TransparentColor(),
      WIDGET_WINDOW_FG, gDisplay.renderBitmap->TransparentColor(),
      WIDGET_SLIDER_FG, COLOR_TEXT_BG,
      WIDGET_SLIDER_BG, COLOR_TEXT,
      WIDGET_END_TAG);

  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_BG, 255, 92, 93);
}

GResetOptionsState::~GResetOptionsState() {
  delete mFont16;
}


