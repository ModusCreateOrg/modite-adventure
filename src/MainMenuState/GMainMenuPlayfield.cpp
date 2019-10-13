#include "GMainMenuPlayfield.h"

GMainMenuPlayfield::GMainMenuPlayfield() {
  gResourceManager.LoadBitmap(TITLE_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBackground = gResourceManager.GetBitmap(BKG_SLOT);
  // gResourceManager.LoadBitmap(LOGO_BMP, PLAYER_SLOT, IMAGE_ENTIRE);
  // mLogo = gResourceManager.GetBitmap(PLAYER_SLOT);

  gDisplay.SetPalette(mBackground);
  gDisplay.SetColor(COLOR_TEXT, WHITE);
  gDisplay.SetColor(COLOR_TEXT_SHADOW, ROSE);
  gDisplay.SetColor(COLOR_TEXT_BG, RED);

  gWidgetTheme.Configure(
    WIDGET_TEXT_BG, COLOR_TEXT_BG,
    WIDGET_TITLE_FONT, gFont16x16,
    WIDGET_TITLE_FG, COLOR_TEXT,
    WIDGET_TITLE_BG, COLOR_TEXT_SHADOW,
    WIDGET_WINDOW_BG, gDisplay.renderBitmap->TransparentColor(),
    WIDGET_WINDOW_FG, gDisplay.renderBitmap->TransparentColor(),
    WIDGET_END_TAG
  );
}

GMainMenuPlayfield::~GMainMenuPlayfield() {
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
  // gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
}

void GMainMenuPlayfield::Render() {
  gDisplay.renderBitmap->CopyPixels(mBackground);
}
