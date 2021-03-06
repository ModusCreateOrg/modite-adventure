#include "GCancelResetWidget.h"
#include "Game.h"

GCancelResetWidget::GCancelResetWidget() : BButtonWidget("Back", COLOR_TEXT, COLOR_TEXT_BG) {}

GCancelResetWidget::~GCancelResetWidget() = default;

TInt GCancelResetWidget::Render(TInt aX, TInt aY) {
  const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);

  if (mActive) {
    gDisplay.renderBitmap->DrawStringShadow(ENull,
        STR_RIGHT_ARROW,
        f,
        aX - 16, aY,
        gWidgetTheme.GetInt(WIDGET_TEXT_BG),
        COLOR_TEXT_SHADOW,
        COLOR_TEXT_TRANSPARENT);
  }

  gDisplay.renderBitmap->DrawStringShadow(ENull,
      mText,
      f,
      aX, aY,
      gWidgetTheme.GetInt(WIDGET_TITLE_FG),
      COLOR_TEXT_SHADOW,
      COLOR_TEXT_TRANSPARENT,
      -6);

  return f->mHeight << 1;
}

void GCancelResetWidget::Select() {
  gGame->SetState(GAME_STATE_MAIN_OPTIONS);
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuOut();
#endif
}
