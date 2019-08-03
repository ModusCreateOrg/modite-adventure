#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GPlayerProcess.h"

GGameState::GGameState() : BGameEngine(gViewPort) {
  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, 10 * 32) - 1, MIN(SCREEN_HEIGHT, 7 * 32) - 1));
  gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);
  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, EXAMPLE_FILELIST_TXT_MAP);
  mPlayerProcess = new GPlayerProcess(this, mGamePlayfield);
  AddProcess(mPlayerProcess);
}

GGameState::~GGameState() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
}

void GGameState::PreRender() {
 gDisplay.renderBitmap->Clear();
}

