#include "GDebugMenuContainer.h"
#include "GLevelWidget.h"
#include "GDebugWidget.h"
#include "GHealthWidget.h"
#include "GItemWidget.h"
#include "GGameState.h"
#include "Items.h"

static const TUint8 WIDGETS_PER_SCREEN = 9;

GDebugMenuContainer::GDebugMenuContainer(TInt aX, TInt aY, BGameEngine *aState) : GDialogWidget("DEBUG MODE", aX, aY) {
  mTimer = 30;

  AddWidget((BWidget &) *new GDebugWidget());
  AddWidget((BWidget &) *new GHealthWidget());

  const TUint8 itemCount = sizeof(items) / sizeof(TInt);
  for (TUint8 i = 1; i < itemCount; i++) {
    AddWidget((BWidget &) *new GItemWidget(i, aState));
  }

  for (TUint8 i = 0; i < NUM_DUNGEONS; i++) {
    const TDungeonInfo *d = &gDungeonDefs[i];
    const TUint8 levels = sizeof(d->mInfo.map) / sizeof(TInt16);

    for (TInt16 j = 0; j < levels; j++) {
      if (d->mInfo.map[j] == -1) {
        break;
      }
      AddWidget((BWidget &) *new GLevelWidget(i, j));
    }
  }
}

GDebugMenuContainer::~GDebugMenuContainer() {}

void GDebugMenuContainer::AddWidget(BWidget &aWidget) {
  mLength++;
  GDialogWidget::AddWidget(aWidget);
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
