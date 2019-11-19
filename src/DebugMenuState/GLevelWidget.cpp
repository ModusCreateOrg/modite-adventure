#include "GLevelWidget.h"
#include "GPlayer.h"

GLevelWidget::GLevelWidget(TInt aLevel, TInt aDepth) : GButtonWidget("") {
  mLevel = aLevel;
  mDepth = aDepth;
  char s[128];
  sprintf(s, "%s - Level %d", gDungeonDefs[mLevel].name, mDepth);
  mText = strdup(s);
}

GLevelWidget::~GLevelWidget() {
  delete mText;
}

TInt GLevelWidget::Render(TInt aX, TInt aY) {
  aY += 20;
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GLevelWidget::Select() {
  // Simulate start button press to exit menu
  gControls.dKeys |= BUTTON_MENU;
  ((GGameState*) gGameEngine)->NextLevel(mLevel, mDepth);
  gViewPort->mWorldX = 0;
  gViewPort->mWorldY = 0;
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuAccept();
#endif
}
