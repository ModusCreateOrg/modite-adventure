#include "GGamePlayfield.h"

GGamePlayfield::GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId) : BMapPlayfield(aViewPort, aTileMapId) {
  gDisplay.SetPalette(this->mTileset);
}

GGamePlayfield::~GGamePlayfield() {
  //
}