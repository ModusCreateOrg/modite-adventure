#include "Game.h"
#include "GGameMenuContainer.h"

#ifdef __XTENSA__
#define PAUSE_MODAL_Y 50
#else
#define PAUSE_MODAL_Y 60
#endif

GGameMenuState::GGameMenuState() : BGameEngine(gViewPort) {
  mContainer = new GGameMenuContainer(20, PAUSE_MODAL_Y);

  gWidgetTheme.Configure(
    WIDGET_TEXT_BG, COLOR_TEXT_BG,
    WIDGET_TITLE_FONT, gFont16x16,
    WIDGET_TITLE_FG, COLOR_TEXT,
    WIDGET_TITLE_BG, COLOR_TEXT_SHADOW,
    WIDGET_SLIDER_FG, COLOR_TEXT_BG,
    WIDGET_SLIDER_BG, COLOR_TEXT,
    WIDGET_WINDOW_BG, gDisplay.renderBitmap->TransparentColor(),
    WIDGET_WINDOW_FG, gDisplay.renderBitmap->TransparentColor(),
    WIDGET_END_TAG
  );

  gDisplay.SetColor(COLOR_TEXT, WHITE);
  gDisplay.SetColor(COLOR_TEXT_SHADOW, ROSE);
  gDisplay.SetColor(COLOR_TEXT_BG, RED);
}

GGameMenuState::~GGameMenuState() {
  delete mContainer;
}

void GGameMenuState::PreRender() {
  // Reset colors before rendering,
  // the main game process may have overwritten them
  gDisplay.SetColor(COLOR_TEXT, WHITE);
  gDisplay.SetColor(COLOR_TEXT_SHADOW, ROSE);
  gDisplay.SetColor(COLOR_TEXT_BG, RED);

  mContainer->Run();
}

void GGameMenuState::PostRender() {
  mContainer->Render(30, 20);
}
