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
    gDisplay.renderBitmap->DrawStringShadow(ENull,
        STR_RIGHT_ARROW,
        f,
        aX - 16, aY,
        gWidgetTheme.GetInt(WIDGET_TEXT_BG),
        COLOR_TEXT_SHADOW,
        COLOR_TEXT_TRANSPARENT);
  }

  gDisplay.renderBitmap->DrawStringShadow(ENull,
      mTitle,
      f,
      aX, aY,
      gWidgetTheme.GetInt(WIDGET_TITLE_FG),
      COLOR_TEXT_SHADOW,
      COLOR_TEXT_TRANSPARENT,
      -6);

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

  const BFont  *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);
  const TUint8 fg = gWidgetTheme.GetInt(WIDGET_SLIDER_FG),
               bg = gWidgetTheme.GetInt(WIDGET_SLIDER_BG);

  for (TInt i = 0; i < 8; i++) {
    gDisplay.renderBitmap->DrawFastVLine8(ENull, aX + 1, aY - 1, 16, COLOR_TEXT_SHADOW);
    gDisplay.renderBitmap->DrawFastHLine8(ENull, aX + 2, aY + 14, 7, COLOR_TEXT_SHADOW);
    gDisplay.renderBitmap->FillRect8(ENull, aX + 2, aY - 2, aX + 9, aY + 13, i < mSelectedValue ? fg : bg);
    aX += 16;
  }

  return f->mHeight - 4;
}
