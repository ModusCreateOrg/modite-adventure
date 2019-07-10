#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GPlayerProcess.h"

GGameState::GGameState() : BGameEngine(gViewPort) {
  gResourceManager.LoadBitmap(DUNGEON2_TILES_BMP, BKG_SLOT, IMAGE_16x16);
  gResourceManager.LoadBitmap(PLAYER_BMP, PLAYER_SLOT, IMAGE_32x32);
  gResourceManager.LoadRaw(DUNGEON2_DUNGEON1_LAYER_LAYER_1_MAP001_STM, MAP_SLOT);
  gResourceManager.LoadRaw(DUNGEON1_TLC, CODES_SLOT);
  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, BKG_SLOT, MAP_SLOT, CODES_SLOT);
  mPlayerProcess = new GPlayerProcess(this, mGamePlayfield);
  AddProcess(mPlayerProcess);
}

GGameState::~GGameState() {
  gResourceManager.ReleaseRawSlot(CODES_SLOT);
  gResourceManager.ReleaseRawSlot(MAP_SLOT);
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
}


