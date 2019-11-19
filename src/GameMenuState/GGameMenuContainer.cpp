#include "GGameMenuContainer.h"
#include "GMusicWidget.h"
#include "GSfxWidget.h"
#include "GBrightnessWidget.h"
#include "GSaveWidget.h"
#include "GResumeWidget.h"

GGameMenuContainer::GGameMenuContainer(TInt aX, TInt aY, GGameState *aGameState) : GDialogWidget("GAME PAUSED", aX, aY) {
  mGameState = aGameState;
#ifdef __XTENSA__
  AddWidget((BWidget &) *new GBrightnessWidget());
#endif
  AddWidget((BWidget &) *new GMusicWidget());
  AddWidget((BWidget &) *new GSfxWidget());
  AddWidget((BWidget &) *new GSaveWidget(mGameState));
  AddWidget((BWidget &) *new GResumeWidget());

  mTimer = 30;
}

GGameMenuContainer::~GGameMenuContainer() {}

TInt GGameMenuContainer::Render(TInt aX, TInt aY) {
  TUint8 color = gWidgetTheme.GetInt(WIDGET_TEXT_BG);;
  const BFont *f = gWidgetTheme.GetFont(WIDGET_TITLE_FONT);
  const TInt x = (SCREEN_WIDTH - (strlen(mTitle) * f->mWidth)) / 2;

  if (--mTimer < 0) {
    mTimer = 30;
  } else if (mTimer >= 15) {
    color = gWidgetTheme.GetInt(WIDGET_TEXT_BG);
  } else if (mTimer < 15) {
    color = gWidgetTheme.GetInt(WIDGET_TITLE_FG);
  }

  gDisplay.renderBitmap->DrawStringShadow(
      ENull,
      mTitle,
      f,
      aX + x, aY + 16,
      color,
      COLOR_TEXT_SHADOW,
      COLOR_TEXT_TRANSPARENT,
      -6
      );

  return GDialogWidget::Render(aX, aY);
}
