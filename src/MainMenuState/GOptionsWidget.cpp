#include "GOptionsWidget.h"
#include "Game.h"

GOptionsWidget::GOptionsWidget() : BButtonWidget("OPTIONS", COLOR_TEXT, COLOR_TEXT_BG) {}

GOptionsWidget::~GOptionsWidget() {}

TInt GOptionsWidget::Render(TInt aX, TInt aY) {
  const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);

  if (mActive) {
    gDisplay.renderBitmap->DrawStringShadow(ENull,
        STR_RIGHT_ARROW,
        f,
        aX - 16, aY,
        RED,
        ROSE
        );
  }

  gDisplay.renderBitmap->DrawStringShadow(ENull,
      mText,
      f,
      aX, aY,
      WHITE,
      ROSE,
      -6);

  return f->mHeight + 10;
}

void GOptionsWidget::Select() {
  gGame->SetState(GAME_STATE_MAIN_OPTIONS);
  // TODO: @jaygarcia
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuAccept();
#endif
}
