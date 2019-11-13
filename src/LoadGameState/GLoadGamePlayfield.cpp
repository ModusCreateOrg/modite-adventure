#include "GLoadGamePlayfield.h"
#include <Display.h>
#include "GResources.h"

GLoadGamePlayfield::GLoadGamePlayfield() {
  //
}

GLoadGamePlayfield::~GLoadGamePlayfield() {
  //
}

void GLoadGamePlayfield::Render() {
  gDisplay.SetColor(COLOR_TEXT, 255, 255,255);
  gDisplay.SetColor(COLOR_TEXT_BG, 0,0,0);
  gDisplay.renderBitmap->Clear(COLOR_TEXT_BG);
}
