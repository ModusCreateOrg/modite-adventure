#include "GDebugMenuContainer.h"
#include "GLevelWidget.h"
#include "GDebugWidget.h"
#include "GHealthWidget.h"
#include "GItemWidget.h"
#include "GDebugButtonWidget.h"
#include "GGameState.h"
#include "Items.h"

static const TUint8 WIDGETS_PER_SCREEN = 14;

GDebugMenuContainer::GDebugMenuContainer(TInt aX, TInt aY, BGameEngine *aEngine) : GDialogWidget("", aX, aY) {
  mTimer = 30;
  mEngine = aEngine;
  SetState(DEBUG_MAIN);
}

GDebugMenuContainer::~GDebugMenuContainer() {}

void GDebugMenuContainer::SetState(TInt aState) {
  mSkip = 0;
  mDir = 0;
  mBuffer = 0;
  mState = aState;

  mList.Reset();
  mLength = 0;

  if (mCurrentWidget) {
    mCurrentWidget->Deactivate();
    delete mCurrentWidget;
  }

  if (mState == DEBUG_MAIN) {
    mTitle = (char*)"DEBUG MODE";
    AddWidget((BWidget &) *new GDebugWidget());
    AddWidget((BWidget &) *new GHealthWidget());
    AddWidget((BWidget &) *new GDebugButtonWidget("DEBUG LEVELS", DEBUG_LEVEL, this));
    AddWidget((BWidget &) *new GDebugButtonWidget("DEBUG INVENTORY", DEBUG_INV, this));
  } else if (mState == DEBUG_LEVEL) {
    mTitle = (char*)"DEBUG LEVELS";
    for (TUint8 i = 0; i < NUM_DUNGEONS; i++) {
      const TDungeonInfo *d = &gDungeonDefs[i];
      const TUint8 levels = sizeof(d->mInfo.map) / sizeof(TInt16);
      for (TInt16 j = 0; j < levels; j++) {
        if (d->mInfo.map[j] == -1) break;
        AddWidget((BWidget &) *new GLevelWidget(i, j));
      }
    }
    AddWidget((BWidget &) *new GDebugButtonWidget("BACK", DEBUG_MAIN, this));
  } else if (mState == DEBUG_INV) {
    mTitle = (char*)"DEBUG INVENTORY";
    const TUint8 itemCount = sizeof(items) / sizeof(TInt);
    for (TUint8 i = 1; i < itemCount; i++) {
      AddWidget((BWidget &) *new GItemWidget(i, mEngine));
    }
    AddWidget((BWidget &) *new GDebugButtonWidget("BACK", DEBUG_MAIN, this));
  } else {
    Panic("Invalid debug state");
  }

  mCurrentWidget = mList.First();
  mCurrentWidget->Activate();
}

void GDebugMenuContainer::AddWidget(BWidget &aWidget) {
  mLength++;
  GDialogWidget::AddWidget(aWidget);
}

void GDebugMenuContainer::Run() {
  for (BWidget *w = (BWidget *) mList.First(); !mList.End(w); w = (BWidget *) mList.Next(w)) {
    w->Run();
  }

  if (gControls.WasPressed(JOYUP) && OnNavigate(JOYUP) && mCurrentWidget->OnNavigate(JOYUP)) {
    mCurrentWidget->Deactivate();
    if (mCurrentWidget == mList.First()) {
      mCurrentWidget = (BWidget *) mList.Last();
    } else {
      mCurrentWidget = (BWidget *) mList.Prev(mCurrentWidget);
    }
    mCurrentWidget->Activate();

    // reset dKeys so next state doesn't react to any keys already pressed
    gControls.dKeys = 0;
  }

  if (gControls.WasPressed(JOYDOWN) && OnNavigate(JOYDOWN) && mCurrentWidget->OnNavigate(JOYDOWN)) {
    mCurrentWidget->Deactivate();
    if (mCurrentWidget == mList.Last()) {
      mCurrentWidget = (BWidget *) mList.First();
    } else {
      mCurrentWidget = (BWidget *) mList.Next(mCurrentWidget);
    }
    mCurrentWidget->Activate();

    // reset dKeys so next state doesn't react to any keys already pressed
    gControls.dKeys = 0;
  }

  if (gControls.WasPressed(JOYLEFT) && OnNavigate(JOYLEFT) && mCurrentWidget->OnNavigate(JOYLEFT)) {
    mCurrentWidget->Deactivate();
    mCurrentWidget = mList.First();
    mCurrentWidget->Activate();
    gControls.dKeys = 0;
  }

  if (gControls.WasPressed(JOYRIGHT) && OnNavigate(JOYRIGHT) && mCurrentWidget->OnNavigate(JOYRIGHT)) {
    mCurrentWidget->Deactivate();
    mCurrentWidget = mList.Last();
    mCurrentWidget->Activate();
    gControls.dKeys = 0;
  }
}

TBool GDebugMenuContainer::OnNavigate(TUint16 aBits) {
  if (aBits & JOYUP) {
    if (mDir > 0) {
      mBuffer = mBuffer ? WIDGETS_PER_SCREEN - mBuffer : WIDGETS_PER_SCREEN;
    }
    if (mBuffer > 0) {
      mBuffer--;
    } else {
      mSkip--;
    }
    mDir = -1;
  }
  else if (aBits & JOYDOWN) {
    if (mDir < 0) {
      mBuffer = mBuffer ? WIDGETS_PER_SCREEN - mBuffer : WIDGETS_PER_SCREEN;
    }
    if (mBuffer > 0) {
      mBuffer--;
    } else {
      mSkip++;
    }
    mDir = 1;
  }

  return ETrue;
}

TInt GDebugMenuContainer::Render(TInt aX, TInt aY) {
  TUint8 color = gWidgetTheme.GetInt(WIDGET_TEXT_BG);
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

  if (mCurrentWidget == mList.First()) {
    mBuffer = 0;
    mSkip = 0;
    mDir = 0;
  } else if (mCurrentWidget == mList.Last()) {
    mBuffer = WIDGETS_PER_SCREEN;
    mSkip = mLength - 1;
    mDir = 0;
  }

  TInt h = 0;
  TInt i = 0;
  for (BWidget *w = mList.First(); !mList.End(w); w = mList.Next(w), i++) {
    if (i + WIDGETS_PER_SCREEN < mSkip) {
      continue;
    }

    TInt dy = w->RenderTitle(aX + mX, aY + mY, w == mCurrentWidget);
    aY += dy;
    h += dy;

    dy =w->Render(aX + mX, aY + mY);  // render relative to mY
    aY += dy;
    h += dy;
  }

  return h;
}
