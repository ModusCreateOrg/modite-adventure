#include "GGamePlayfield.h"

GGamePlayfield::GGamePlayfield(BViewPort *aViewPort, TUint16 aTileMapId) : BMapPlayfield(aViewPort, aTileMapId) {
  gDisplay.SetPalette(this->mTileset, 0, 128);
  auto bm = gResourceManager.GetBitmap(PLAYER_SLOT);
  gDisplay.SetPalette(bm->GetPalette(), PLAYER_PALETTE, SPIDER_COLORS);
  bm = gResourceManager.GetBitmap(SPIDER_SLOT);
  gDisplay.SetPalette(bm->GetPalette(), SPIDER_PALETTE, SPIDER_COLORS);
}

GGamePlayfield::~GGamePlayfield() {
  //
}