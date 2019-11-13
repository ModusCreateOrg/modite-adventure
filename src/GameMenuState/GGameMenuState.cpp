#include "Game.h"
#include "GGameMenuContainer.h"

#ifdef __XTENSA__
#define PAUSE_MODAL_Y 50
#else
#define PAUSE_MODAL_Y 60
#endif

GGameMenuState::GGameMenuState(GGameState *aGameState) : BGameEngine(gViewPort) {
  mGameState = aGameState;
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
  mContainer = new GGameMenuContainer(20, PAUSE_MODAL_Y, mGameState);

  gWidgetTheme.Configure(
      WIDGET_TEXT_FONT, mFont16,
      WIDGET_TEXT_FG, COLOR_TEXT,
      WIDGET_TEXT_BG, COLOR_TEXT_BG,
      WIDGET_TITLE_FONT, mFont16,
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
    c.r = c.g = c.b = max;
    // set grayscale color in palette
    gDisplay.SetColor(color, c);
  }

  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  gDisplay.SetColor(COLOR_TEXT_BG, 255, 92, 93);
}

GGameMenuState::~GGameMenuState() {
  gDisplay.SetPalette(mSavedPalette);
  delete mFont16;
  delete mContainer;
}

void GGameMenuState::PreRender() {
  mContainer->Run();
}

void GGameMenuState::PostRender() {
  mContainer->Render(30, 20);
}
