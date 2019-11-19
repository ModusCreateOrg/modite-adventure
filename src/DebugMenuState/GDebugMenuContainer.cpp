#include "GDebugMenuContainer.h"
#include "GLevelWidget.h"
#include "GDebugWidget.h"
#include "GHealthWidget.h"
#include "GGameState.h"

GDebugMenuContainer::GDebugMenuContainer(TInt aX, TInt aY) : GDialogWidget("DEBUG MODE", aX, aY) {
  AddWidget((BWidget &) *new GDebugWidget());
  AddWidget((BWidget &) *new GHealthWidget());

  for (TUint8 i = 0; i < NUM_DUNGEONS; i++) {
    const TDungeonInfo d = dungeon_defs[i];
    const TUint8 levels = sizeof(d.mInfo.map) / sizeof(TUint16);

    for (TInt16 j = 0; j < levels; j++) {
      if (d.mInfo.map[j] == -1) {
        break;
      }
      AddWidget((BWidget &) *new GLevelWidget(i, j));
    }
  }
  mTimer = 30;
}

GDebugMenuContainer::~GDebugMenuContainer() {}

TInt GDebugMenuContainer::Render(TInt aX, TInt aY) {
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
