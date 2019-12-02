#include "GLevelWidget.h"
#include "GPlayer.h"

GLevelWidget::GLevelWidget(TInt aLevel, TInt aDepth) : GButtonWidget("") {
  mLevel = aLevel;
  mDepth = aDepth;
  char s[128];
  const char *f = ((GGameState*) gGameEngine)->IsCurrentLevel(aLevel, aDepth)
    ? "* %s - Level %d"
    : "  %s - Level %d";
  sprintf(s, f, gDungeonDefs[mLevel].name, mDepth);
  mText = strdup(s);
}

GLevelWidget::~GLevelWidget() {
  delete mText;
}

TInt GLevelWidget::Render(TInt aX, TInt aY) {
  aY += 20;
  aX -= 20;
  GButtonWidget::Render(aX, aY);
  return gWidgetTheme.GetFont(WIDGET_TITLE_FONT)->mHeight - 4;
}

void GLevelWidget::Select() {
  if (((GGameState*) gGameEngine)->IsCurrentLevel(mLevel, mDepth)) {
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxMenuOut();
#endif
    return;
  }
  // Simulate start button press to exit menu
  gControls.dKeys |= CONTROL_DEBUG;
  ((GGameState*) gGameEngine)->NextLevel(mLevel, mDepth);
  gViewPort->mWorldX = 0;
  gViewPort->mWorldY = 0;
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxMenuIn();
#endif
}
