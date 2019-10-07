#include "Game.h"
#include "GStartWidget.h"
#include "GMainMenuProcess.h"

GStartWidget::GStartWidget() : BButtonWidget("START") {
  if (gOptions->gameProgress.savedState) {
    mText = (char*)"CONTINUE";
  }
}

GStartWidget::~GStartWidget() {}

TInt GStartWidget::Render(TInt aX, TInt aY) {
  const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);

  if (mActive) {
    gDisplay.renderBitmap->DrawStringShadow(ENull,
        STR_RIGHT_ARROW,
        f,
        aX - 16, aY,
        gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TEXT_BG)),
        gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_BG)));
  }

  gDisplay.renderBitmap->DrawStringShadow(ENull,
      mText,
      f,
      aX, aY,
      gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_FG)),
      gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_BG)),
      -6);

  return f->mHeight + 10;
}

void GStartWidget::Select() {
  gGame->SetState(GAME_STATE_GAME);
// TODO: @jaygarcia
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuAccept();
#endif
}
