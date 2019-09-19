#include "GGamePlayfield.h"

GGamePlayfield::GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId) : BMapPlayfield(aViewPort, aTileMapId, TILESET_SLOT) {
  gDisplay.SetPalette(this->mTileset, 0, 128);
}

GGamePlayfield::~GGamePlayfield() {
  //
}

