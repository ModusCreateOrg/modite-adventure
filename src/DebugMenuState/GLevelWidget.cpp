#include "GLevelWidget.h"
#include "GPlayer.h";

GLevelWidget::GLevelWidget(TInt aLevel, TInt aDepth) : GButtonWidget("") {
  mLevel = aLevel;
  mDepth = aDepth;
}

GLevelWidget::~GLevelWidget() {}

TInt GLevelWidget::Render(TInt aX, TInt aY) {
#ifdef __XTENSA__
  aY += 16;
#else
  aY += 20;
#endif
  // @TODO move the below to constructor (causes SEGFAULT - couldn't figure out)
  char s[128];
  snprintf(s, sizeof(s), "%s - Level %d", dungeon_defs[mLevel].name, mDepth);
  mText = s;
  // end todo
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
