#include "Game.h"
#include "GMusicWidget.h"
#include "GSfxWidget.h"
#include "GResetWidget.h"
#include "GExitWidget.h"

#ifdef __XTENSA__
#include "GBrightnessWidget.h"
#define WIDGET_CONTAINER_Y 50
#else
#define WIDGET_CONTAINER_Y 60
#endif

class OptionsContainer : public GDialogWidget {
public:
    OptionsContainer(TInt aX, TInt aY) : GDialogWidget("Options", aX, aY) {
#ifdef __XTENSA__
      AddWidget((BWidget &) *new GBrightnessWidget());
#endif
      AddWidget((BWidget &) *new GMusicWidget());
      AddWidget((BWidget &) *new GSfxWidget());
      AddWidget((BWidget &) *new GResetWidget());
      AddWidget((BWidget &) *new GExitWidget());
    }
};

class GMainOptionsProcess : public BProcess {
public:
    GMainOptionsProcess() : BProcess() {
      mContainer = new OptionsContainer(10, WIDGET_CONTAINER_Y);
    }

    ~GMainOptionsProcess() {
      delete mContainer;
    }

    TBool RunBefore() {
      mContainer->Render(30, 20);
      mContainer->Run();
      return ETrue;
    }

    TBool RunAfter() {
      if (gControls.WasPressed(BUTTON_MENU | BUTTON_START)) {
        gGame->SetState(GAME_STATE_MAIN_MENU);
        // TODO: @jaygarcia
#ifdef ENABLE_AUDIO
        gSoundPlayer.SfxMenuCancel();
#endif
        return EFalse;
      }
      return ETrue;
    }

protected:
    OptionsContainer *mContainer;
};

class GMainOptionsPlayfield : public BPlayfield {
public:
    GMainOptionsPlayfield() {
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

    virtual ~GMainOptionsPlayfield() {
      gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
    }

    void Render() {
      gDisplay.renderBitmap->CopyPixels(mBackground);
    }

    BBitmap *mBackground;
};


GMainOptionsState::GMainOptionsState() : BGameEngine(gViewPort) {
  mPlayfield = new GMainOptionsPlayfield();
  AddProcess(new GMainOptionsProcess());
}

GMainOptionsState::~GMainOptionsState() {}


