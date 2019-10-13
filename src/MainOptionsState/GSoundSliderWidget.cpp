#include "GSoundSliderWidget.h"
#include "Game.h"

GSoundSliderWidget::GSoundSliderWidget(char *aTitle, const TRange *aRange, TInt aForeground, TInt aBackground)
    : BSliderWidget(aTitle, aRange, aForeground, aBackground) {
}

GSoundSliderWidget::GSoundSliderWidget(const char *aTitle, const TRange *aRange, TInt aForeground, TInt aBackground)
    : BSliderWidget(aTitle, aRange, aForeground, aBackground) {
}

GSoundSliderWidget::~GSoundSliderWidget() {}

void GSoundSliderWidget::Set(TInt aVal) {
  mSelectedValue = aVal;
}

TInt GSoundSliderWidget::RenderTitle(TInt aX, TInt aY, TBool aActive) {
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
    mTitle,
    f,
    aX,
    aY,
    gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_FG)),
    gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_BG)),
    -6
  );

#ifdef __XTENSA__
  return f->mHeight;
#else
  return f->mHeight + 4;
#endif
}

TInt GSoundSliderWidget::Render(TInt aX, TInt aY) {
  aX += 82;
#ifdef __XTENSA__
  aY -= 16;
#else
  aY -= 20;
#endif

  const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);
  const TRGB  fg = gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_SLIDER_FG)),
              bg = gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_SLIDER_BG)),
              sh = gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_BG));

  for (TInt i = 0; i < 8; i++) {
    gDisplay.renderBitmap->DrawFastVLine(gViewPort, aX + 1, aY - 1, 16, sh);
    gDisplay.renderBitmap->DrawFastHLine(gViewPort, aX + 2, aY + 14, 7, sh);
    gDisplay.renderBitmap->FillRect(gViewPort, aX + 2, aY - 2, aX + 9, aY + 13, i < mSelectedValue ? fg : bg);
    aX += 16;
  }

  return f->mHeight - 4;
}
