#include "GGameMenuContainer.h"
#include "GMusicWidget.h"
#include "GSfxWidget.h"
#include "GBrightnessWidget.h"
#include "GSaveWidget.h"
#include "GResumeWidget.h"
#include "GQuitWidget.h"
#include "GOptionsWidget.h"

GGameMenuContainer::GGameMenuContainer(TInt aX, TInt aY, GGameState *aGameState) : GDialogWidget("GAME PAUSED", aX, aY) {
  mGameState = aGameState;
  mTimer = 30;
  mState = GAME_MENU_PAUSE_STATE;
}

GGameMenuContainer::~GGameMenuContainer() {}

void GGameMenuContainer::State(GAME_MENU_STATE aState) {
  mState = aState;
}

void GGameMenuContainer::PauseState() {
  Clear();
  State(GAME_MENU_KEEP_STATE);
  mTitle = (char *)"Game Paused";

  AddWidget((BWidget &) *new GResumeWidget());
  if (gGame->IsGameState() && !((GGameState *) gGameEngine)->IsBossRoom()) {
    AddWidget((BWidget &) *new GSaveWidget(mGameState));
  }
  AddWidget((BWidget &) *new GOptionsWidget(this, EFalse));
  AddWidget((BWidget &) *new GQuitWidget());

  mCurrentWidget = mList.First();
  mList.First()->Activate();
}

void GGameMenuContainer::OptionsState() {
  Clear();
  State(GAME_MENU_KEEP_STATE);
  mTitle = (char *)"Volume";

  AddWidget((BWidget &) *new GMusicWidget());
  AddWidget((BWidget &) *new GSfxWidget());
  AddWidget((BWidget &) *new GOptionsWidget(this, ETrue));

  mCurrentWidget = mList.First();
  mList.First()->Activate();
}

void GGameMenuContainer::Clear() {
  BWidget *w = (BWidget *) mList.First();
  while (!(BWidget *) mList.End(w)) {
    BWidget *n = (BWidget *) mList.Next(w);
    delete w;
    w = n;
  }
  mList.Reset();
}

TInt GGameMenuContainer::Render(TInt aX, TInt aY) {
  switch (mState) {
    case GAME_MENU_KEEP_STATE:
      break;
    case GAME_MENU_PAUSE_STATE:
      PauseState();
      break;
    case GAME_MENU_OPTIONS_STATE:
      OptionsState();
      break;
    default:
      Panic("Invalid Game Menu state\n");
  }

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
