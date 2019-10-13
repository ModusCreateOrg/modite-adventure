#include "GGameMenuContainer.h"
#include "GMusicWidget.h"
#include "GSfxWidget.h"
#include "GBrightnessWidget.h"
#include "GSaveWidget.h"
#include "GResumeWidget.h"

GGameMenuContainer::GGameMenuContainer(TInt aX, TInt aY) : GDialogWidget("GAME PAUSED", aX, aY) {
#ifdef __XTENSA__
  AddWidget((BWidget &) *new GBrightnessWidget());
#endif
  AddWidget((BWidget &) *new GMusicWidget());
  AddWidget((BWidget &) *new GSfxWidget());
  AddWidget((BWidget &) *new GSaveWidget());
  AddWidget((BWidget &) *new GResumeWidget());

  mTimer = 30;
}

GGameMenuContainer::~GGameMenuContainer() {}

TInt GGameMenuContainer::Render(TInt aX, TInt aY) {
  TInt index = gWidgetTheme.GetInt(WIDGET_TITLE_FG);
  const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);
  const TInt x = (SCREEN_WIDTH - (strlen(mTitle) * f->mWidth)) / 2;

  if (--mTimer < 0) {
    mTimer = 30;
  } else if (mTimer >= 15) {
    index = gWidgetTheme.GetInt(WIDGET_TEXT_BG);
  } else if (mTimer < 15) {
    index = gWidgetTheme.GetInt(WIDGET_TITLE_FG);
  }

  gDisplay.renderBitmap->DrawStringShadow(
    ENull,
    mTitle,
    f,
    aX + x,
    aY + 16,
    gDisplay.GetColor(index),
    gDisplay.GetColor(gWidgetTheme.GetInt(WIDGET_TITLE_BG)),
    -6
  );

  return GDialogWidget::Render(aX, aY);
}
