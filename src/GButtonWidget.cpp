#include "GButtonWidget.h"

GButtonWidget::GButtonWidget(const char *aText, TUint aState) : BButtonWidget("") {
  mText = (char *) aText;
  mState = aState;
}

GButtonWidget::~GButtonWidget() {}

TInt GButtonWidget::Render(TInt aX, TInt aY) {
  const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);

  if (mActive) {
    gDisplay.renderBitmap->DrawStringShadow(
      gViewPort,
      STR_RIGHT_ARROW,
      f,
      aX - 16,
      aY,
      gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TEXT_BG)),
      gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_BG))
    );
  }

  gDisplay.renderBitmap->DrawStringShadow(
    gViewPort,
    mText,
    f,
    aX,
    aY,
    gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_FG)),
    gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_BG)),
    -6
  );

  return f->mHeight + 10;
}

void GButtonWidget::Select() {
  gGame->SetState(mState);
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuAccept();
#endif
}
