#include "Game.h"
#include "GDebugMenuContainer.h"

GDebugMenuState::GDebugMenuState() : BGameEngine(gViewPort) {
  mContainer = new GDebugMenuContainer(30, 40, this);

  gWidgetTheme.Configure(
      WIDGET_TEXT_FONT, gFont16x16,
      WIDGET_TEXT_FG, COLOR_TEXT,
      WIDGET_TEXT_BG, COLOR_TEXT_BG,
      WIDGET_TITLE_FONT, gFont16x16,
      WIDGET_TITLE_FG, COLOR_TEXT,
      WIDGET_TITLE_BG, -1,
      WIDGET_WINDOW_BG, gDisplay.renderBitmap->TransparentColor(),
      WIDGET_WINDOW_FG, gDisplay.renderBitmap->TransparentColor(),
      WIDGET_SLIDER_FG, COLOR_TEXT_BG,
      WIDGET_SLIDER_BG, COLOR_TEXT,
      WIDGET_END_TAG);

  TRGB *source = gDisplay.renderBitmap->GetPalette();
  for (TInt color = 0; color < COLOR_TEXT; color++) {
    TRGB c = source[color];
    mSavedPalette[color] = c;
    // convert color to grayscale color
    TUint8 max = MAX(c.b, MAX(c.r, c.g));
    c.r = c.g = c.b = (max > 0) ? (max * .15) : 0;
    // set grayscale color in palette
    gDisplay.SetColor(color, c);
  }


  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_BG, 255, 92, 93);
  gDisplay.SetColor(COLOR_TEXT_SHADOW, 255, 0, 0);
}

GDebugMenuState::~GDebugMenuState() {
  gDisplay.SetPalette(mSavedPalette);

  delete mContainer;
}

void GDebugMenuState::PreRender() {
  mContainer->Run();
}

void GDebugMenuState::PostRender() {
  mContainer->Render(30, 10);
}
