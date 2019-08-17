#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GPlayerProcess.h"
#include "GSpiderProcess.h"

GGameState::GGameState() : BGameEngine(gViewPort) {
  gViewPort->SetRect(TRect(0, 0, MIN(SCREEN_WIDTH, TILES_WIDE * 32) - 1, MIN(SCREEN_HEIGHT, TILES_HIGH * 32) - 1));

  // load and remap player
  gResourceManager.LoadBitmap(CHARA_HERO_BMP, PLAYER_SLOT, IMAGE_64x64);
  BBitmap *bm = gResourceManager.GetBitmap(PLAYER_SLOT);
  bm->Remap(PLAYER_PALETTE, PLAYER_COLORS);

  // load and remap spider
  gResourceManager.LoadBitmap(CHARA_SPIDER_BMP, SPIDER_SLOT, IMAGE_64x64);
  bm = gResourceManager.GetBitmap(SPIDER_SLOT);
  bm->Remap(SPIDER_PALETTE, SPIDER_COLORS);

  mPlayerProcess = new GPlayerProcess(this);
  AddProcess(mPlayerProcess);
  LoadLevel(EXAMPLE_FILELIST_TXT_MAP);
}

GGameState::~GGameState() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
}

void GGameState::PreRender() {
  gDisplay.renderBitmap->Clear();
}

TUint16 GGameState::MapWidth() {
  return (mGamePlayfield->MapWidthTiles() - TILES_WIDE) * 32;
}

TUint16 GGameState::MapHeight() {
  return (mGamePlayfield->MapHeightTiles() - TILES_HIGH) * 32;
}

void GGameState::LoadLevel(TUint16 aTileMapId) {
  if (mPlayfield) {
    delete mPlayfield;
  }
  mPlayfield = mGamePlayfield = new GGamePlayfield(gViewPort, EXAMPLE_FILELIST_TXT_MAP);
  TInt objectCount = mGamePlayfield->mObjectCount;
  TUint16 *program = mGamePlayfield->mObjectProgram;

  for (TInt ip = 0; ip < objectCount*3; ip += 3) {
    TUint16 op = program[ip],
      op1 = program[ip + 1],  // row
      op2 = program[ip + 2];  // col

    auto xx = TFloat(op2 * 32),
      yy = TFloat(op1 * 32);

    switch (op) {
      case ATTR_PLAYER:
        printf("PLAYER at %f,%f\n", xx, yy);
        mPlayerProcess->StartLevel(mGamePlayfield, xx-16, yy);
        break;
      case ATTR_SPIDER:
        printf("SPIDER at %f,%f %d %d\n", xx, yy, op1, op2);
        AddProcess(new GSpiderProcess(this, mGamePlayfield, xx, yy+63));
        break;
      default:
        printf("Invalid op code in Object Program: %x\n", program[ip]);
        break;
    }
  }
}

