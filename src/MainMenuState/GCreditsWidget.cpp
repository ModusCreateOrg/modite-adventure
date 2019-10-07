#include "GCreditsWidget.h"
#include "Game.h"

GCreditsWidget::GCreditsWidget() : BButtonWidget("CREDITS") {}

GCreditsWidget::~GCreditsWidget() {}

TInt GCreditsWidget::Render(TInt aX, TInt aY) {
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

void GCreditsWidget::Select() {
  gGame->SetState(GAME_STATE_CREDITS);
// TODO: @jaygarcia
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuAccept();
#endif
}
